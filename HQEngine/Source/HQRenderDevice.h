/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef _HQ_RDEVICE_
#define _HQ_RDEVICE_

#include "HQPlatformDef.h"
#include "HQRendererPlatformDef.h"

/*-------Win 32 ----------*/
#if defined WIN32 && !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
#	include "windows.h"

#	define hModule HMODULE
#	define HQRenderDeviceInitInput HWND
#   define DLLDECL

/*-------Win Phone ----------*/
#elif (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
#	include "windows.h"
#	include <agile.h>

#	define hModule void*
#	define HQRenderDeviceInitInput Platform::Agile<Windows::UI::Core::CoreWindow>
#   define DLLDECL


/*------Linux-----------*/
#elif defined (HQ_LINUX_PLATFORM)
#   include<X11/X.h>
#   include<X11/Xlib.h>

#	define hModule void*
#   define DLLDECL __attribute__ ((visibility("default")))

typedef struct HQXWindowInfo{
    const char * title;
    int x, y;//toa do window
    Window parent ;//can be DefaultRootWindow(display)
    Window window;//this must be 0 when passing to HQRenderDevice::Init() method, a new window will be created and stored in this member
} HQX11RenderDeviceInitInput , *HQRenderDeviceInitInput;


/*------Iphone OS-----------*/
#elif defined HQ_IPHONE_PLATFORM

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#	define hModule void*
#   define DLLDECL __attribute__ ((visibility("default")))

typedef struct HQIOSRenderDeviceInitInput {
	HQIOSRenderDeviceInitInput (CAEAGLLayer *_eaglLayer , bool _landscapeMode) 
	: eaglLayer(_eaglLayer) , landscapeMode(_landscapeMode)
	{}
	HQIOSRenderDeviceInitInput (CAEAGLLayer *_eaglLayer) 
	: eaglLayer(_eaglLayer) , landscapeMode(false)
	{}
	HQIOSRenderDeviceInitInput()
	: eaglLayer(nil) , landscapeMode(false)
	{}
	
	CAEAGLLayer *eaglLayer;
	bool landscapeMode;//is landscape mode
} *HQRenderDeviceInitInput;


/*------MacOS-----------*/
#elif defined HQ_MAC_PLATFORM

#	define hModule void*
#   define DLLDECL __attribute__ ((visibility("default")))

#import <Cocoa/Cocoa.h>

typedef struct HQOSXRenderDeviceInitInput {
	HQOSXRenderDeviceInitInput (NSView *_nsView , bool windowed) 
	: nsView(_nsView) , isWindowed(windowed)
	{}
	HQOSXRenderDeviceInitInput (NSView *_nsView) 
	: nsView(_nsView) , isWindowed(true)
	{}
	HQOSXRenderDeviceInitInput()
	: nsView(nil) , isWindowed(true)
	{}
	
	NSView *nsView;
	bool isWindowed;
} *HQRenderDeviceInitInput;

/*---------android----------*/
#elif defined HQ_ANDROID_PLATFORM

#	if defined ANDROID_PURE_NATIVE//pure native
#		error need implement
#	else//default

#include <jni.h>

typedef struct HQAndroidRenderDeviceInitInput
{
	jint apiLevel;//openGL ES version 1 or 2.other values are equal to 1
	jobject jengineView;//global reference to hqengine.java.HQEngineView object
	jobject jegl;//global reference to javax.microedition.khronos.egl.EGL10 object
	jobject jdisplay;//global reference to javax.microedition.khronos.egl.EGLDisplay object
}*HQRenderDeviceInitInput;

#	endif

#endif
/*------end operating system switch----*/

#include <stdio.h>
#include <fstream>
#include "HQLogStream.h"
#include "HQRendererCoreType.h"
#include "HQReturnVal.h"
#include "HQTextureManager.h"
#include "HQVertexStreamManager.h"
#include "HQRendererStateManager.h"
#include "HQShaderManager.h"
#include "HQRenderTargetManager.h"
#include "HQUtilMath.h"




class HQRenderDevice{
public:
	HQRenderDevice(){};

	virtual HQReturnVal Release()=0;
	virtual bool IsRunning()=0;
	///
	///kiểm tra device có đang trong tình trạng "lost" không. Cần kiểm tra thường xuyên. 
	///Sau khi device trở lại trạng thái bình thường, dữ liệu bên trong các vertex/index buffer , các render target cần được reset. 
	///Ngoài ra trong android : phải kiểm tra trước khi gọi các method khác của device và texture, shader cần được load lại sau khi 
	///device trở lại trạng thái bình thường. Khi device ở trạng thái "lost" không nên gọi các method liên quan đến device
	///
	virtual bool IsDeviceLost() = 0;
	
	///
	///{settingFileDir} - đường dẫn đến file setting cơ bản lưu các thông số độ phân giải , refresh rate .v..v.v có thể NULL => tùy chọn mặc định
	///{additionalSettings} - tùy chọn thêm , các tùy chọn cách nhau bằng khoảng cách.
	///	Ví dụ : "Core-GL4.2" - Khởi tạo OpenGL device dùng version 4.2 (trở lên) core profile nếu có thể. Version có thể là bất kỳ lớn hơn 3.0.  
	///			"GLSL-only" - openGL device shader manager sẽ chỉ dùng GLSL shader. 
	///			"CG-only" - openGL device shader manager sẽ chỉ dùng CG shader.
	///			nếu cả GLSL-only & CG-only đều trong chuỗi option , option xuất hiện sau trong chuỗi sẽ được chọn.
	///
	virtual HQReturnVal Init(HQRenderDeviceInitInput input,const char* settingFileDir, HQLogStream* logStream , const char *additionalSettings = NULL)=0;
	
	///
	///nếu {clearWholeRenderTarget} = false , chỉ vùng viewport bị clear ,
	///ngược lại toàn bộ màn hình hoặc toàn bộ bề mặt render target(nếu đang dùng render target không phải mặc định) sẽ bị clear
	///
	virtual HQReturnVal BeginRender(HQBool clearPixel,HQBool clearDepth,HQBool clearStencil,HQBool clearWholeRenderTarget=HQ_FALSE)=0;
	virtual HQReturnVal EndRender()=0;
	///
	/// hiển thị hình ảnh trong backbuffer lên màn hình
	///
	virtual HQReturnVal DisplayBackBuffer() = 0;

	///
	///truy vấn chiều rộng của vùng sẽ hiển thị hình ảnh mà device render
	///
	virtual hq_uint32 GetWidth()=0;
	///
	///truy vấn chiều cao của vùng sẽ hiển thị hình ảnh mà device render
	///
	virtual hq_uint32 GetHeight()=0;
	virtual bool IsWindowed()=0;
	virtual bool IsVSyncEnabled() = 0;
	///
	///truy vấn kiểu siêu lấy mẫu mà device đang dùng khi render lên back buffer
	///
	virtual HQMultiSampleType GetMultiSampleType() = 0;
	
	///
	///nếu {clearWholeRenderTarget} = false , chỉ vùng viewport bị clear ,
	///ngược lại toàn bộ màn hình hoặc toàn bộ bề mặt render target(nếu đang dùng render target không phải mặc định) sẽ bị clear
	///
	virtual HQReturnVal Clear(HQBool clearPixel,HQBool clearDepth,HQBool clearStencil,HQBool clearWholeRenderTarget=HQ_FALSE)=0;
	///
	///color range:0.0f->1.0f
	///
	virtual void SetClearColorf(hq_float32 red,hq_float32 green,hq_float32 blue,hq_float32 alpha)=0;
	inline void SetClearColor(const HQColor & color)
	{
		SetClearColorf(color.r, color.g, color.b, color.a);
	}
	///
	///color range:0->255
	///
	virtual void SetClearColori(hq_uint32 red,hq_uint32 green,hq_uint32 blue,hq_uint32 alpha)=0;


	///
	///{val} trong khoảng [0 - 1]
	///
	virtual void SetClearDepthVal(hq_float32 val)=0;
	///
	///set giá trị mà stencil buffer sẽ được clear thành giá trị này
	///
	virtual void SetClearStencilVal(hq_uint32 val)=0;

	virtual void GetClearColor(HQColor &clearColorOut) const = 0;

	virtual hqfloat32 GetClearDepthVal() const = 0;

	virtual hquint32 GetClearStencilVal() const = 0;

#if !defined HQ_OPENGLES && !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
#define HQ_RENDER_RESOLUTION_CHANGEABLE 1
	///
	///nếu {resolutionList} = NULL, số độ phân giải sẽ được lưu vào {numResolutions}, 
	///ngược lại , dãy {resolutionList} với tối đa {numResolutions} phần tử sẽ được lưu các độ phân giản hỗ trợ của device
	///
	virtual void GetAllDisplayResolution(HQResolution *resolutionList , hq_uint32& numResolutions) = 0;
	///
	///Chuyển đổi chế độ fullscreen/windowed thất bại nếu device là openGL device.
	///Nếu dùng direct3d 9 device , cần reset lại dữ liệu trong các buffer và render target
	///
	virtual HQReturnVal SetDisplayMode(hq_uint32 width,hq_uint32 height,bool windowed)=0;
#endif

	///
	///Gọi method này để resize back buffer khi render window thay đổi kích thước
	///Nếu dùng direct3d 9 device , cần reset lại dữ liệu trong các buffer và render target
	///
	virtual HQReturnVal OnWindowSizeChanged(hq_uint32 width,hq_uint32 height)=0;

	///
	///nếu vùng viewport không nằm gọn trong phạm vi của render target, viewport sẽ được set thành toàn bộ vùng render của render target
	///
	virtual HQReturnVal SetViewPort(const HQViewPort &viewport) = 0;
	
	virtual const HQViewPort & GetViewPort() const = 0;

	///
	///nếu dùng direct3d 9 device , cần reset lại dữ liệu trong các buffer và render target.
	///
	virtual void EnableVSync(bool enable)=0;

	///
	///set phần tử cơ sở khi render. 
	///mặc định HQ_PRI_TRIANGLES
	///
	virtual void SetPrimitiveMode(HQPrimitiveMode primitiveMode) = 0;
	///
	///render không dùng index buffer.
	///{firstVertex} là chỉ số đỉnh đầu tiên dùng trong vertex buffer
	///
	virtual HQReturnVal Draw(hq_uint32 vertexCount , hq_uint32 firstVertex) = 0;
	///
	///render không dùng index buffer.
	///{firstVertex} là chỉ số đỉnh đầu tiên dùng trong vertex buffer. 
	///Direct3D9 dùng method này ít overhead hơn Draw(), nhưng các API khác gặp nhiều overhead hơn
	///
	virtual HQReturnVal DrawPrimitive(hq_uint32 primitiveCount , hq_uint32 firstVertex) = 0;
	///
	///render dùng index buffer.
	///{numVertices} là số đỉnh trong vertex buffer từ đình đầu tiên đến đỉnh có index max trong số các index dùng trong index buffer. 
	///{firstIndex} là chỉ số index đầu tiên dùng trong index buffer
	///
	virtual HQReturnVal DrawIndexed(hq_uint32 numVertices , hq_uint32 indexCount , hq_uint32 firstIndex ) = 0;
	///
	///render dùng index buffer. 
	///{numVertices} là số đỉnh trong vertex buffer từ đình đầu tiên đến đỉnh có index max trong số các index dùng trong index buffer. 
	///{firstIndex} là chỉ số index đầu tiên dùng trong index buffer. 
	///Direct3D9 dùng method này ít overhead hơn DrawIndexed(), nhưng các API khác gặp nhiều overhead hơn
	///
	virtual HQReturnVal DrawIndexedPrimitive(hq_uint32 numVertices , hq_uint32 primitiveCount , hq_uint32 firstIndex ) = 0;

	/*-------------------------------
	device capabilities
	-----------------------------*/
	virtual HQColorLayout GetColoruiLayout() = 0;
	virtual hq_uint32 GetMaxVertexStream() = 0;
	///
	///get max vertex attributes
	///
	virtual hq_uint32 GetMaxVertexAttribs() = 0;
	virtual bool IsVertexAttribDataTypeSupported(HQVertexAttribDataType dataType) = 0;
	virtual bool IsIndexDataTypeSupported(HQIndexDataType iDataType) = 0;
	///
	///truy vấn số texture sampler unit nhiều nhất có thể dùng ở tất cả shader stage
	///
	virtual hq_uint32 GetMaxShaderSamplers() = 0;
	///
	///truy vấn số texture sampler nhiều nhất có thể dùng trong shader stage {shaderStage}
	///
	virtual hq_uint32 GetMaxShaderStageSamplers(HQShaderType shaderStage) = 0;
	///
	///truy vấn số texture nhiều nhất có thể dùng ở tất cả shader stage
	///
	virtual hq_uint32 GetMaxShaderTextures() = 0;
	///
	///truy vấn số texture nhiều nhất có thể dùng trong shader stage {shaderStage}
	///
	virtual hq_uint32 GetMaxShaderStageTextures(HQShaderType shaderStage) = 0;
	///
	///is two sided stencil supported
	///
	virtual bool IsTwoSideStencilSupported() = 0;
	///
	///is extended blend state supported
	///
	virtual bool IsBlendStateExSupported() = 0 ;
	virtual bool IsTextureBufferFormatSupported(HQTextureBufferFormat format) = 0;

	///
	///are non power of 2 textures fully supported. That means it is not needed to use HQ_TAM_CLAMP address mode 
	///and point and linear filder on these textures, as well as it is not required that these texture are not compressed.
	///
	virtual bool IsNpotTextureFullySupported(HQTextureType textureType) = 0;
	///
	///are non power of 2 textures supported. It doesn't mean that they are fully supported. 
	///Call IsNpotTextureFullySupported() to check
	///
	virtual bool IsNpotTextureSupported(HQTextureType textureType) = 0;
	
	///
	///truy vấn khả năng hỗ trợ shader. 
	///Ví dụ IsShaderSupport(VERTEX_SHADER,"2.0"). 
	///Direct3D 9: format {major.minor} major và minor 1 chữ số. 
	///Direct3D 10/11 : format {major.minor}. 
	///OpenGL : format {major.minor}.Thực chất là kiểm tra GLSL version
	virtual bool IsShaderSupport(HQShaderType shaderType,const char* version)=0;

	///
	///check if render target texture can be created with format {format}. 
	///{hasMipmaps} - this texture has full range mipmap or not
	///
	virtual bool IsRTTFormatSupported(HQRenderTargetFormat format , HQTextureType textureType ,bool hasMipmaps) = 0;
	///
	///check if depth stencil buffer can be created with format {format}
	///
	virtual bool IsDSFormatSupported(HQDepthStencilFormat format) = 0;
	///
	///check if render target texture can be created with multi sample type {multisampleType}
	///
	virtual bool IsRTTMultisampleTypeSupported(HQRenderTargetFormat format ,
											   HQMultiSampleType multisampleType ,
											   HQTextureType textureType) = 0;
	///
	///check if depth stencil buffer can be created with multi sample type {multisampleType}
	///
	virtual bool IsDSMultisampleTypeSupported(HQDepthStencilFormat format ,
											  HQMultiSampleType multisampleType) = 0;

	///
	///return max number of render targets can be active at a time
	///
	virtual hq_uint32 GetMaxActiveRenderTargets() = 0;

	///
	///check if mipmaps generation for render target texture is supported
	///
	virtual bool IsRTTMipmapGenerationSupported() = 0;
	/*-----------------------
	misc commands
	---------------------*/
	inline HQTextureManager *GetTextureManager() {return textureMan;}
	inline HQVertexStreamManager* GetVertexStreamManager() {return vStreamMan;}
	inline HQShaderManager *GetShaderManager() {return shaderMan;}
	inline HQRendererStateManager *GetStateManager() {return stateMan;}
	inline HQRenderTargetManager *GetRenderTargetManager() {return renderTargetMan;}
	
	///
	///truy vấn tọa độ của 1 điểm trong hệ tọa độ màn hình từ 1 điểm có tọa độ vPos trong hệ tọa độ thế giới. 
	///hệ tọa độ màn hình gốc ở góc trên trái. 
	///ma trận dạng row major. 
	///Lưu ý : -tọa độ màn hình không đồng nghĩa với tọa độ pixel . 
	///		  -Direct3d 9 , tọa độ màn hình = tọa độ pixel. 
	///		  -Direct3d 1x / OpenGL , tọa độ màn hình bằng tọa độ pixel + 0.5 pixel size . 
	///Ví dụ pixel đầu tiên trong màn hình có tọa độ pixel (0,0) thì toạ độ màn hình của nó là (0.5 , 0.5). 
	///<viewProj>: row major
	///
	virtual void GetScreenCoordr(const HQMatrix4 &viewProj , const HQVector4& vPos , HQPoint<hqint32> &pointOut)=0;
	
	///
	///truy vấn tia trong hệ tọa độ thế giới . 
	///tia đi từ tâm camera và có hướng theo như điểm đặt trong hệ tọa độ màn hình (point),vector hướng của tia chưa chuẩn hóa. 
	///hệ tọa độ màn hình gốc ở góc trên trái. 
	///ma trận dạng row major. 
	///Lưu ý : -tọa độ màn hình không đồng nghĩa với tọa độ pixel . 
	///		  -Direct3d 9 , tọa độ màn hình = tọa độ pixel. 
	///		  -Direct3d 1x / OpenGL , tọa độ màn hình bằng tọa độ pixel + 0.5 pixel size . 
	///Ví dụ pixel đầu tiên trong màn hình có tọa độ pixel (0,0) thì toạ độ màn hình của nó là (0.5 , 0.5). 
	///<view> & <proj>: row major
	///
	virtual void GetRayr(const HQMatrix4 &view ,const HQMatrix4 &proj , hq_float32 zNear,
						const HQPoint<hqint32>& point, HQRay3D & rayOut)=0;

	///
	///truy vấn tên mô tả của device.
	///Các giá trị có thể trả về:
	///-"Direct3D 9".
	///-"Direct3D 11".
	///-"OpenGL".
	///-"OpenGL ES".
	///
	virtual const char * GetDeviceDesc() = 0;

protected:
	virtual ~HQRenderDevice(){};
	
	HQTextureManager * textureMan;
	HQVertexStreamManager *vStreamMan;
	HQRenderTargetManager *renderTargetMan;
	HQRendererStateManager * stateMan;
	HQShaderManager *shaderMan;
};

typedef HQRenderDevice *LPHQRenderDevice;
typedef HQTextureManager *LPHQTextureManager;
typedef HQVertexStreamManager *LPHQVertexStreamManager;
typedef HQRendererStateManager *LPHQRendererStateManager;
typedef HQShaderManager *LPHQShaderManager;
typedef HQRenderTargetManager *LPHQRenderTargetManager;




#endif
