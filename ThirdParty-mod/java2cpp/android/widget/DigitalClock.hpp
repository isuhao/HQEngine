/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: android.widget.DigitalClock
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_DECL
#define J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_DECL


namespace j2cpp { namespace android { namespace content { class Context; } } }
namespace j2cpp { namespace android { namespace widget { class TextView; } } }
namespace j2cpp { namespace android { namespace util { class AttributeSet; } } }


#include <android/content/Context.hpp>
#include <android/util/AttributeSet.hpp>
#include <android/widget/TextView.hpp>


namespace j2cpp {

namespace android { namespace widget {

	class DigitalClock;
	class DigitalClock
		: public object<DigitalClock>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)
		J2CPP_DECLARE_METHOD(3)

		explicit DigitalClock(jobject jobj)
		: object<DigitalClock>(jobj)
		{
		}

		operator local_ref<android::widget::TextView>() const;


		DigitalClock(local_ref< android::content::Context > const&);
		DigitalClock(local_ref< android::content::Context > const&, local_ref< android::util::AttributeSet > const&);
	}; //class DigitalClock

} //namespace widget
} //namespace android

} //namespace j2cpp

#endif //J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_IMPL
#define J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_IMPL

namespace j2cpp {



android::widget::DigitalClock::operator local_ref<android::widget::TextView>() const
{
	return local_ref<android::widget::TextView>(get_jobject());
}


android::widget::DigitalClock::DigitalClock(local_ref< android::content::Context > const &a0)
: object<android::widget::DigitalClock>(
	call_new_object<
		android::widget::DigitalClock::J2CPP_CLASS_NAME,
		android::widget::DigitalClock::J2CPP_METHOD_NAME(0),
		android::widget::DigitalClock::J2CPP_METHOD_SIGNATURE(0)
	>(a0)
)
{
}



android::widget::DigitalClock::DigitalClock(local_ref< android::content::Context > const &a0, local_ref< android::util::AttributeSet > const &a1)
: object<android::widget::DigitalClock>(
	call_new_object<
		android::widget::DigitalClock::J2CPP_CLASS_NAME,
		android::widget::DigitalClock::J2CPP_METHOD_NAME(1),
		android::widget::DigitalClock::J2CPP_METHOD_SIGNATURE(1)
	>(a0, a1)
)
{
}





J2CPP_DEFINE_CLASS(android::widget::DigitalClock,"android/widget/DigitalClock")
J2CPP_DEFINE_METHOD(android::widget::DigitalClock,0,"<init>","(Landroid/content/Context;)V")
J2CPP_DEFINE_METHOD(android::widget::DigitalClock,1,"<init>","(Landroid/content/Context;Landroid/util/AttributeSet;)V")
J2CPP_DEFINE_METHOD(android::widget::DigitalClock,2,"onAttachedToWindow","()V")
J2CPP_DEFINE_METHOD(android::widget::DigitalClock,3,"onDetachedFromWindow","()V")

} //namespace j2cpp

#endif //J2CPP_ANDROID_WIDGET_DIGITALCLOCK_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
