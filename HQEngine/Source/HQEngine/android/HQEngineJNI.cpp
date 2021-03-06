/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#include "../../HQEngineApp.h"
#include "../../HQConditionVariable.h"
#include "../../HQAtomic.h"
#include "../HQEventSeparateThread.h"

#include <jni.h>
#include <pthread.h>

#include <unistd.h>

#include <j2cpp/j2cpp.hpp>
#include <j2cpp/raw_environment.hpp>
#include <android/view/MotionEvent.hpp>
#include <android/app/Activity.hpp>
#include <android/view/View.hpp>
#define J2CPP_INCLUDE_IMPLEMENTATION
#include <android/view/MotionEvent.hpp>
#include <android/app/Activity.hpp>
#include <android/view/View.hpp>


#define LOG_TIME_SPEND_IN_LOCK 0

#if LOG_TIME_SPEND_IN_LOCK
#include <android/log.h>
#endif

#ifndef min
#	define min(a, b) ((a > b)? a : b)
#endif

using namespace j2cpp;

struct HQTouchDataNativeBuffer
{
	HQTouchData touchData;
	jint type;
};

//engine activity
static jobject g_jactivity;

//for init jni
JavaVM *ge_jvm;
static pthread_key_t g_javaDetach = 0;
static void OnJavaDetach(void* arg);

//for runnable running on ui thread
static jclass g_juiThreadTaskClass;
static jmethodID g_registerNativeFuncID;
typedef void (*pf_funcRunOnUiThread) (void);


//for opengl
jobject ge_jview;//hqengine.java.HQEngineView object
jobject ge_jegl;//EGL10 object
jobject ge_jdisplay;//EGLDisplay object

//for event handling
extern HQEventQueue* hq_engine_eventQueue_internal;

//for manually stop activity when game thread exit
extern HQEngineAppListener hq_internalDefaultAppListener;

//for controlling app's status flag
#define USE_MUTEX 0

int hq_internalAppNormalState = 0;
int hq_internalAppExit = 1;
int hq_internalAppIsPaused = 2;

#if USE_MUTEX
static int g_appStatusFlag = hq_internalAppNormalState;
static HQMutex g_mutex;
#else

//for blocking game thread
static HQSimpleConditionVar g_pauseGameThreadCnd;
static HQAtomic<bool> g_game_thread_blocked = false;

# include <sys/atomics.h>

static volatile int g_appStatusFlag = hq_internalAppNormalState;
#endif

//for check if surface is valid for creating opengl context
#if USE_MUTEX
static int g_surfaceCreated = 0;
static HQMutex g_mutex2;
#else
static volatile int g_surfaceCreated = 0;
#endif

/*------------jni init-----------------*/

extern "C" {
	jint __attribute__ ((visibility("default"))) JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		ge_jvm = vm;
		
		j2cpp::shared_ptr<j2cpp::raw_environment> raw_env(new j2cpp::raw_environment(ge_jvm));
		
		j2cpp::environment::init(raw_env);
		
		//create thread destructor
		pthread_key_create(&g_javaDetach, OnJavaDetach);
		
		return JNI_VERSION_1_2;
	}
}

///this will be called when attached thread exit
void OnJavaDetach(void* arg)
{
    ge_jvm->DetachCurrentThread();
    pthread_setspecific(g_javaDetach, NULL);
}

JNIEnv * AttachCurrenThreadJEnv()
{
	JNIEnv *env;
	if (ge_jvm->AttachCurrentThread(&env, NULL)!= JNI_OK)
		return NULL;
	
	//set thread key value so OnJavaDetach() will be called when attached thread exit
	pthread_setspecific(g_javaDetach, env);
		
	return env;
}		

/*----------control app's status flag-----------*/

static void HQAppInternalSetStatusFlag(int flag)
{
#if USE_MUTEX
	HQMutex::ScopeLock sl(g_mutex);
	g_appStatusFlag = flag;
#else
	__atomic_swap(flag, &g_appStatusFlag);
#endif
}


int HQAppInternalGetStatusFlag()
{
#if USE_MUTEX
	HQMutex::ScopeLock sl(g_mutex);
	return g_appStatusFlag;
#else
	return __sync_fetch_and_or(&g_appStatusFlag, 0x0);
#endif
}

/*----------game thread blocking control--------*/

void HQAppInternalBlockGameLoopIfNeeded()
{
	if (!g_pauseGameThreadCnd.TryLock())
		return;
    
	if (HQAppInternalGetStatusFlag() == hq_internalAppIsPaused)
	{
		g_game_thread_blocked = true;
        
		g_pauseGameThreadCnd.Wait();
        
	}
    
	g_pauseGameThreadCnd.Unlock();
}

static void HQAppInternalWakeGameLoopIfNeededNoLock()
{
	bool shouldWake = false;
    
	shouldWake = g_game_thread_blocked;
	g_game_thread_blocked = false;
    
	if (shouldWake)
		g_pauseGameThreadCnd.Signal();
    
}


static void HQAppInternalWakeGameLoopIfNeeded()
{
	g_pauseGameThreadCnd.Lock();
	
	HQAppInternalWakeGameLoopIfNeededNoLock();
    
	g_pauseGameThreadCnd.Unlock();
    
}

/*----------for manually stop activity when game thread exit while activity is still running----------*/
void HQAppInternalOnGameThreadExit()
{
	if (HQAppInternalGetStatusFlag() != hq_internalAppExit)
	{
		HQEngineApp *pEngineApp = HQEngineApp::GetInstance();
		if (pEngineApp != NULL)
		{
			pEngineApp->SetAppListener(hq_internalDefaultAppListener);
		}
		android::app::Activity acitivityCpp(g_jactivity);
		acitivityCpp.finish();
	}
}

/*------------check if surface is valid for creating opengl context-----*/
static void HQEngineInternalSetIsJSurfaceCreated(int flag)
{
#if USE_MUTEX
	HQMutex::ScopeLock sl(g_mutex2);
	g_surfaceCreated = flag;
#else
	__atomic_swap(flag, &g_surfaceCreated);
#endif
}


int HQEngineInternalIsJSurfaceCreated()
{
#if USE_MUTEX
	HQMutex::ScopeLock sl(g_mutex2);
	return g_surfaceCreated;
#else
	return __sync_fetch_and_or(&g_surfaceCreated, 0x0);
#endif
}

/*----------set view's visibility----------*/
void HQEngineInternalSetViewVisibility(jobject jview, bool visible)
{
	android::view::View viewCpp(jview);
	if (visible)
		viewCpp.setVisibility(android::view::View::VISIBLE);
	else
		viewCpp.setVisibility(android::view::View::INVISIBLE);
}



/*------------running task on uithread---------------*/
void HQEngineInternalRunOnUiThread(void (*runFunc)(void), bool wait )
{
	JNIEnv *env;
	jint re = ge_jvm->GetEnv((void**)&env, JNI_VERSION_1_2);

	if(re == JNI_EDETACHED) 
	{
		env = AttachCurrenThreadJEnv();
	}
	
	jlong funcAddress = (jlong) runFunc;
	env->CallStaticVoidMethod(	
						g_juiThreadTaskClass ,
						g_registerNativeFuncID,
						funcAddress,
						wait? JNI_TRUE: JNI_FALSE);
	
}

extern "C"
{
/*--------------HQEngineBaseActivity native methods-----*/
	static jint GetActionIndex(jint action)
	{
#ifdef ANDROID_SDK_API_8
		return (action & android::view::MotionEvent::ACTION_POINTER_INDEX_MASK) >> android::view::MotionEvent::ACTION_POINTER_INDEX_SHIFT;
#else
		return (action & android::view::MotionEvent::ACTION_POINTER_ID_MASK) >> android::view::MotionEvent::ACTION_POINTER_ID_SHIFT;
#endif
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onCreateNativeInternal
	  (JNIEnv *env, jobject thiz, jobject viewObject, jobject egl, jobject display)
	{
		//store global ref to activity object
		g_jactivity = env->NewGlobalRef(thiz);
		
		//create global ref	
		ge_jview = env->NewGlobalRef(viewObject);
		ge_jegl = env->NewGlobalRef(egl);
		ge_jdisplay = env->NewGlobalRef(display);
		
		//set application normal state flag
		HQAppInternalSetStatusFlag(hq_internalAppNormalState);

		//start game thread
		ge_hqGameThread.Start();
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_setCurrentDir(JNIEnv *env, jobject thiz, jstring path)
	{
		/*-----------change current directory---------------*/
		//copy path
		const char *nativeString = env->GetStringUTFChars(path, 0);

		chdir(nativeString);
		
		env->ReleaseStringUTFChars(path, nativeString);
	}

	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onTouchNative
	  (JNIEnv *env, jobject thiz, jobject touchDataBuffer)
	{
		HQTouchDataNativeBuffer * pTouchDataBuffer = (HQTouchDataNativeBuffer*) env->GetDirectBufferAddress(touchDataBuffer);
		
		//add to event queue
#if LOG_TIME_SPEND_IN_LOCK
		HQTime dt;
		HQTimeCheckPoint t1, t2;
		HQTimer::GetCheckPoint(t1);
#endif
		HQEventQueue::ScopeLock sl(hq_engine_eventQueue_internal);

#if LOG_TIME_SPEND_IN_LOCK
		HQTimer::GetCheckPoint(t2);
		dt = HQTimer::GetElapsedTime(t1 , t2);
		__android_log_print(ANDROID_LOG_DEBUG, "HQEngineJNI", "time spend waiting for event queue : %f", dt);
#endif	

		
		HQEvent &newEvent = hq_engine_eventQueue_internal->BeginAddEvent();
		//copy event
		newEvent.type = (HQEventType)pTouchDataBuffer->type;
		newEvent.touchData.numTouches = min(pTouchDataBuffer->touchData.numTouches, MAX_MULTITOUCHES);
		for (hquint32 i = 0 ; i < newEvent.touchData.numTouches ; ++i)
			newEvent.touchData.touches[i] = pTouchDataBuffer->touchData.touches[i];
			
		hq_engine_eventQueue_internal->EndAddEvent();

		HQThread::TempPause();
	}
	
	JNIEXPORT jboolean JNICALL Java_hqengine_java_HQEngineBaseActivity_onBackKeyPressedNative
	  (JNIEnv *env, jobject thiz)
	{
		HQEngineApp *pEngineApp = HQEngineApp::GetInstance();
		if (pEngineApp != NULL)
		{
			HQEngineAppListener *appListener = pEngineApp->GetAppListener();
			if(appListener->BackButtonPressed())
				return JNI_TRUE;
			else
				return JNI_FALSE;
		}
		
		return JNI_TRUE;
	}

	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onPauseNative
	  (JNIEnv *env, jobject thiz)
	{
		HQAppInternalSetStatusFlag(hq_internalAppIsPaused);
	
		HQEngineApp *pEngineApp = HQEngineApp::GetInstance();
		
		if (pEngineApp != NULL)
		{
			HQEngineAppListener *appListener = pEngineApp->GetAppListener();
			appListener->OnPause();
		}
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onResumeNative
	  (JNIEnv *env, jobject thiz)
	{
		HQAppInternalSetStatusFlag(hq_internalAppNormalState);
        
        HQAppInternalWakeGameLoopIfNeeded();//wake game thread if needed
        
		HQEngineApp *pEngineApp = HQEngineApp::GetInstance();
		
		if (pEngineApp != NULL)
		{
			HQEngineAppListener *appListener = pEngineApp->GetAppListener();
			appListener->OnResume();
		}
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onDestroyNative
	  (JNIEnv *env, jobject thiz)
	{
		HQAppInternalSetStatusFlag(hq_internalAppExit);
	
		HQEngineApp *pEngineApp = HQEngineApp::GetInstance();
		
		if (pEngineApp != NULL)
		{
			HQEngineAppListener *appListener = pEngineApp->GetAppListener();
			appListener->OnDestroy();
		}
		
		//wait for game thread to stop
		ge_hqGameThread.Join();
		
		//delete global ref to activity object
		env->DeleteGlobalRef(g_jactivity);
		
		
		//delete global ref
		
		env->DeleteGlobalRef(ge_jview);
		env->DeleteGlobalRef(ge_jegl);
		env->DeleteGlobalRef(ge_jdisplay);
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onChangedToPortraitNative(JNIEnv *env, jobject thiz)
	{
		HQEventQueue::ScopeLock sl(hq_engine_eventQueue_internal);
		HQEvent &newEvent = hq_engine_eventQueue_internal->BeginAddEvent();
		
		newEvent.type = HQ_ORIENTATION_PORTRAIT;
		
		hq_engine_eventQueue_internal->EndAddEvent();
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onChangedToPortraitUpsideDownNative(JNIEnv *env, jobject thiz)
	{
		HQEventQueue::ScopeLock sl(hq_engine_eventQueue_internal);
		HQEvent &newEvent = hq_engine_eventQueue_internal->BeginAddEvent();
		
		newEvent.type = HQ_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
		
		hq_engine_eventQueue_internal->EndAddEvent();
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onChangedToLandscapeRightNative(JNIEnv *env, jobject thiz)
	{
		HQEventQueue::ScopeLock sl(hq_engine_eventQueue_internal);
		HQEvent &newEvent = hq_engine_eventQueue_internal->BeginAddEvent();
		
		newEvent.type = HQ_ORIENTATION_LANDSCAPE_RIGHT;
		
		hq_engine_eventQueue_internal->EndAddEvent();
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineBaseActivity_onChangedToLandscapeLeftNative(JNIEnv *env, jobject thiz)
	{
		HQEventQueue::ScopeLock sl(hq_engine_eventQueue_internal);
		HQEvent &newEvent = hq_engine_eventQueue_internal->BeginAddEvent();
		
		newEvent.type = HQ_ORIENTATION_LANDSCAPE_LEFT;
		
		hq_engine_eventQueue_internal->EndAddEvent();
	}
/*--------------HQEngineView native methods-------------*/

	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineView_surfaceCreatedNative
	  (JNIEnv *env, jobject thiz)
	{	
		//surface is created
		HQEngineInternalSetIsJSurfaceCreated(1);
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineView_surfaceDestroyedNative
	  (JNIEnv *env, jobject thiz)
	{
		//surface is destroyed
		HQEngineInternalSetIsJSurfaceCreated(0);
	}
	
	/*----------HQEngineUIRunnable native methods------------*/
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineUIRunnable_initNative
	  (JNIEnv *env, jclass clazz)
	{
		//get method id
		g_registerNativeFuncID = env->GetStaticMethodID(
			clazz,
			"registerNativeFunc",
			"(JZ)V"
		);
		
		//create global ref
		g_juiThreadTaskClass = (jclass)env->NewGlobalRef(clazz);
	}
	
	JNIEXPORT void JNICALL Java_hqengine_java_HQEngineUIRunnable_runNative
	  (JNIEnv *env, jobject thiz, jlong funcAddress)
	{
		pf_funcRunOnUiThread funcRunOnUiThread = (pf_funcRunOnUiThread) funcAddress;
		if (funcRunOnUiThread != NULL)
		{
			funcRunOnUiThread();
		}
	}
	
	/*-----------HQEngineTouchDataNative native methods-----------*/
	JNIEXPORT jboolean JNICALL Java_hqengine_java_HQEngineTouchDataNative_isLittleEndian
	  (JNIEnv *env, jclass clazz)
	{
		hqubyte8 array[] = {0, 0, 0, 0xff};
		hquint32 integer = *((hquint32*)array);
		
		if (integer == 0xff)
			return JNI_FALSE;
		return JNI_TRUE;
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getBufferSize
	  (JNIEnv *env, jclass clazz)
	{
		return sizeof(HQTouchDataNativeBuffer);
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getTouchEventTypeOffset
	  (JNIEnv *env, jclass clazz)
	{
		return sizeof(HQTouchData);
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getSingleTouchSize
	  (JNIEnv *env, jclass clazz)
	{
		return sizeof(HQTouch);
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getMaxMultiTouches
	  (JNIEnv *env, jclass clazz)
	{
		return MAX_MULTITOUCHES;
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getTouchBeganTypeVal
	  (JNIEnv *env, jclass clazz)
	{
		return HQ_TOUCH_BEGAN;
	}
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getTouchMovedTypeVal
	  (JNIEnv *env, jclass clazz)
	{
		return HQ_TOUCH_MOVED;
	}
	
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getTouchEndedTypeVal
	  (JNIEnv *env, jclass clazz)
	{
		return HQ_TOUCH_ENDED;
	}
	JNIEXPORT jint JNICALL Java_hqengine_java_HQEngineTouchDataNative_getTouchCancelledTypeVal
	  (JNIEnv *env, jclass clazz)
	{
		return HQ_TOUCH_CANCELLED;
	}
}
