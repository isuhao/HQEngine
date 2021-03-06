/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#include "HQDeviceD3D11PCH.h"
#include "HQDeviceD3D11.h"
#include <string.h>

#define FORCE_UNBIND_BOUND_UAV_BUFFERS 0
#define CS_UAV_SLOTS_CHANGED 0x10000
#define VS_SRV_SLOTS_CHANGED 0x20000
#define GS_SRV_SLOTS_CHANGED 0x40000
#define PS_SRV_SLOTS_CHANGED 0x80000
#define CS_SRV_SLOTS_CHANGED 0x100000
#define VBUFFER_SLOTS_CHANGED 0x200000

#ifndef min
#define min(a, b) (a < b ? a : b)
#endif

#if defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM
#include "..\HQEngine\winstore\HQWinStoreUtil.h"

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
#endif

HQDeviceD3D11* g_pD3DDev=0;

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM 
WNDPROC g_winProc = NULL;
bool g_firstTimePresent = true;
/*---------overridding window procedure-----------------------*/
LRESULT CALLBACK OverridingWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_SIZE:
		{
		}
		break;
	case WM_KILLFOCUS:
		g_pD3DDev->OnWindowActive(false);
		break;
	case WM_SETFOCUS:
		
		if(!g_firstTimePresent)
			g_pD3DDev->OnWindowActive(true);
		
		break;
	}
	return CallWindowProc(g_winProc , hwnd , message , wParam , lParam);
}

#else
static float ConvertDipsToPixels(float dips)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * Windows::Graphics::Display:: DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
}
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM 

//***********************************
//create device
//***********************************
extern "C" {
HQReturnVal CreateDevice(hModule pDll,LPHQRenderDevice *pDev,bool flushDebugLog , bool debugLayer){
	if(g_pD3DDev)
		return HQ_DEVICE_ALREADY_EXISTS;
	*pDev=new HQDeviceD3D11(pDll , flushDebugLog);

	return HQ_OK;
}
}
//**************************************
//safe release device
//**************************************
extern "C" {
HQReturnVal ReleaseDevice(LPHQRenderDevice * ppDev)
{
    HQReturnVal re = HQ_OK;
    if(g_pD3DDev!=NULL)
    {
        re = (*ppDev)->Release();
    }
    *ppDev = NULL;
    return re;
}
}
/*----------------------
HQDeviceD3D11
----------------------*/
HQDeviceD3D11::HQDeviceD3D11(hModule _pDll , bool flushLog)
:HQBaseRenderDevice("Direct3D11" , "D3D11 Render Device :" , flushLog)
{


	pDll=_pDll;

	sWidth=800;
	sHeight=600;

	flags = WINDOWED | CLEAR_COLOR_OR_DEPTH_CHANGED;

	pFactory=0;
	pDevice=0;
	pDevContext = 0;
	pSwapChain = 0;
	pMainRenderTargetView = 0;
	pDepthStencilBuffer = 0;
	pMainDepthStencilView = 0;
	d3dPrimitiveMode = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	
	
	d3dPrimitivelookupTable[HQ_PRI_TRIANGLES] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d3dPrimitivelookupTable[HQ_PRI_TRIANGLE_STRIP] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	d3dPrimitivelookupTable[HQ_PRI_POINT_SPRITES] = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	d3dPrimitivelookupTable[HQ_PRI_LINES] = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	d3dPrimitivelookupTable[HQ_PRI_LINE_STRIP] = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	
	clearColor=HQColorRGBA(0.0f, 0.0f,0.0f, 1.0f);
	clearColorui = clearColor;
	clearDepth=1.0f;
	clearStencil=0;
	
	pEnum=0;

	//-------------compute's deferred UAV slots--------------------
	for (hquint32 i = 0; i < D3D11_PS_CS_UAV_REGISTER_COUNT; ++i)
	{
		this->pDeferredCSUAVSlots[i] = NULL;
		this->CSUAVInitialCounts[i] = -1;
	}
	this->minUsedCSUAVSlot = D3D11_PS_CS_UAV_REGISTER_COUNT;
	this->maxUsedCSUAVSlot = -1;

	memset(pVSSRVSlots, 0, sizeof(pVSSRVSlots));
	memset(pGSSRVSlots, 0, sizeof(pGSSRVSlots));
	memset(pPSSRVSlots, 0, sizeof(pPSSRVSlots));
	memset(pCSSRVSlots, 0, sizeof(pCSSRVSlots));

	this->minUsedVSSRVSlot = this->minUsedGSSRVSlot = this->minUsedPSSRVSlot = this->minUsedCSSRVSlot
		= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

	this->maxUsedVSSRVSlot = this->maxUsedGSSRVSlot = this->maxUsedPSSRVSlot = this->maxUsedCSSRVSlot
		= -1;

	memset(pIAStreamBufferSlots, 0, sizeof(pIAStreamBufferSlots));

	this->minUsedIAStreamSlot = MAX_VERTEX_ATTRIBS;
	this->maxUsedIAStreamSlot = -1;
	
	this->textureMan=0;
	this->vStreamMan=0;
	this->shaderMan=0;
	this->stateMan = 0;
	this->renderTargetMan = 0;

	this->maxNumVPs = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	this->currentVPs = HQ_NEW HQViewPort[this->maxNumVPs];
	this->numUsedViewports = 1;
	

	g_pD3DDev=this;
}
//***********************************
//destructor,release
//***********************************
HQDeviceD3D11::~HQDeviceD3D11(){
#if defined DEBUG || defined _DEBUG
	/*
	ID3D11Debug *pD3DDebug = NULL;
	pDevice->QueryInterface(__uuidof(ID3D11Debug) , (void**)&pD3DDebug);
	*/
#endif

	pDevContext->ClearState();
	pDevContext->Flush();
	
	SafeDelete(pEnum);
	SafeDeleteTypeCast(HQShaderManagerD3D11*, shaderMan);
	SafeDeleteTypeCast(HQTextureManagerD3D11*, textureMan);
	SafeDeleteTypeCast(HQVertexStreamManagerD3D11*, vStreamMan);
	SafeDeleteTypeCast(HQRenderTargetManagerD3D11*, renderTargetMan);
	SafeDeleteTypeCast(HQStateManagerD3D11*, stateMan);

	SafeRelease(pMainRenderTargetView);
	if(!this->IsWindowed() && pSwapChain)
		pSwapChain->SetFullscreenState(FALSE , NULL);
	SafeRelease(pSwapChain);
	SafeRelease(pMainDepthStencilView);
	SafeRelease(pDepthStencilBuffer);

	pDevContext->ClearState();
	pDevContext->Flush();
	
	SafeRelease(pDevContext);

#if defined DEBUG || defined _DEBUG
	/*
	if (pD3DDebug != NULL)
	{
		pD3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		SafeRelease(pD3DDebug);
	}
	*/
#endif
	SafeRelease(pDevice);

	SafeRelease(pFactory);

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM 
	//reset window procedure
	if(winfo.hwind != NULL)
	{
		SetWindowLongA(winfo.hwind , GWL_WNDPROC , (LONG) g_winProc);
	}
	g_winProc = NULL;
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM 

	Log("Released!");
}
HQReturnVal HQDeviceD3D11::Release(){
	if(g_pD3DDev != NULL)
    {
        HQ_DELETE (this);
        g_pD3DDev = NULL;
    }
	return HQ_OK;
}
//***********************************
//init
//***********************************
HQReturnVal HQDeviceD3D11::Init(HQRenderDeviceInitInput _hwnd,const char* settingFileDir, HQLogStream* logFileStream, const char *additionalSettings)
{
	if(this->IsRunning())
	{
		Log("Already init!");
		return HQ_FAILED;
	}
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	if (_hwnd == NULL)
#else
	if (_hwnd == nullptr)
#endif
	{
		Log("Init() is called with invalid parameters!");
		return HQ_FAILED;
	}

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	//window info
	winfo.hwind=_hwnd;
	winfo.styles = (GetWindowLongPtrA(_hwnd,GWL_STYLE) & (~WS_MAXIMIZEBOX));
	winfo.styles &= (~WS_THICKFRAME);
	winfo.hparent = (HWND)GetWindowLongPtrA(_hwnd,GWLP_HWNDPARENT);

	RECT rect;
	GetWindowRect(_hwnd,&rect);
	if(winfo.hparent != 0)
	{
		POINT topleft;
		topleft.x = rect.left;
		topleft.y = rect.top;

		ScreenToClient(winfo.hparent,&topleft);
		winfo.x = topleft.x;
		winfo.y = topleft.y;
	}
	else
	{
		winfo.x = rect.left;
		winfo.y = rect.top;
	}
#else//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	winfo.window = _hwnd;
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM

	this->SetLogStream(logFileStream);
	
	//create IDXGIFactory object
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory) );
#else
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&pFactory) );
#endif
	if(FAILED(hr))
	{
		Log("CreateDXGIFactory() failed!");
		return HQ_FAILED_CREATE_DEVICE;
	}

	//create HQDeviceEnumD3D11 object
	pEnum=new HQDeviceEnumD3D11(pFactory );

	//enum
	pEnum->Enum();
	//parse setting file
	pEnum->ParseSettingFile(settingFileDir);
	if(settingFileDir)
		this->CopySettingFileDir(settingFileDir);

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	sWidth=pEnum->selectedMode->Width;
	sHeight=pEnum->selectedMode->Height;
#else//windows phone uses corewindow size
	//get window size on ui thead
	UINT width, height;
	HQWinStoreUtil::RunOnUIThreadAndWait(winfo.window->Dispatcher,
				[this, &width, &height]()
				{
					width = (hquint32)winfo.window->Bounds.Width;
					height = (hquint32)winfo.window->Bounds.Height;
				});

	sWidth = ConvertDipsToPixels(width);
	sHeight = ConvertDipsToPixels(height);

#if defined HQ_WIN_STORE_PLATFORM
	auto orientation = Windows::Graphics::Display::DisplayProperties::CurrentOrientation;
	bool swapDimensions =
		orientation ==  Windows::Graphics::Display::DisplayOrientations::Portrait ||
		orientation ==  Windows::Graphics::Display::DisplayOrientations::PortraitFlipped;

	if (swapDimensions)
	{
		auto temp = sWidth;
		sWidth = sHeight;
		sHeight = temp;
	}

	pEnum->ChangeSelectedDisplay(sWidth, sHeight, pEnum->windowed);

#endif//#if defined HQ_WIN_STORE_PLATFORM
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM

	if(pEnum->windowed)
		flags |=WINDOWED;
	else
		flags &=(~WINDOWED);
	
	if(pEnum->vsync != 0)
	{
		this->flags |= VSYNC_ENABLE;
	}
	else
		this->flags &= ~VSYNC_ENABLE;

	//save setting to file
	pEnum->SaveSettingFile(settingFileDir);

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	//resize window
	RECT Rect={0,0,sWidth,sHeight};
	if(this->flags & WINDOWED)
	{
		SetWindowLongPtrW(winfo.hwind,GWL_STYLE,winfo.styles);
		AdjustWindowRect(&Rect,winfo.styles,FALSE);
		SetWindowPos(winfo.hwind, HWND_TOP, winfo.x, winfo.y,
			Rect.right-Rect.left, Rect.bottom-Rect.top, SWP_NOZORDER);
	}
	else
	{
		SetParent(winfo.hwind,NULL);
		SetWindowLongPtrW(winfo.hwind,GWL_STYLE,WS_POPUP);
		AdjustWindowRect(&Rect,WS_POPUP,FALSE);
		SetWindowPos(winfo.hwind, HWND_TOP, 0, 0,
			Rect.right-Rect.left, Rect.bottom-Rect.top, SWP_NOZORDER);
	}
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM

	const D3D_FEATURE_LEVEL featureLvls[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	UINT deviceThreadFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#else
	UINT deviceThreadFlags = 0;
#endif

	IDXGIAdapter * l_selectedAdapter;


	{
		//try to create device from selected adapter and driver first then try to create device from default adapter and WARP driver
		IDXGIAdapter * trying_Adapter[] = {
			pEnum->GetSelectedAdapter(),
			NULL,
		};

		D3D_DRIVER_TYPE trying_Drivers[] = 
		{
			pEnum->GetSelectedDriverType(),
			D3D_DRIVER_TYPE_WARP
		};
		
		size_t numTries = sizeof(trying_Drivers) / sizeof(D3D_DRIVER_TYPE);


		for (size_t i = 0; i < numTries; ++i)
		{

			l_selectedAdapter = trying_Adapter[i];

			//create device
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
			hr = D3D11CreateDevice(trying_Adapter[i] ,
				trying_Drivers[i] ,
#else
			ID3D11DeviceContext *context = NULL;
			hr = D3D11CreateDevice(trying_Adapter[i] ,
				trying_Drivers[i],
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
				NULL,
#if !defined HQ_NO_D3D_DEBUG && (defined (DEBUG) || defined (_DEBUG) || defined HQ_D3D_DEBUG) 
				D3D11_CREATE_DEVICE_DEBUG,
#else
				deviceThreadFlags,
#endif
				featureLvls, 3,
				D3D11_SDK_VERSION,
				&pDevice ,
				&featureLvl ,
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
				&pDevContext);
#else
				&context);
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM

			
#if defined (DEBUG) || defined (_DEBUG)
			if (FAILED(hr))//can't create device with debug layer, so create normal device
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
				hr = D3D11CreateDevice(trying_Adapter[i] ,
					trying_Drivers[i] ,
#else
				hr = D3D11CreateDevice(trying_Adapter[i] ,
					trying_Drivers[i] ,
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
					NULL,
					deviceThreadFlags,
					featureLvls, 3,
					D3D11_SDK_VERSION,
					&pDevice ,
					&featureLvl ,
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
				&pDevContext);
#else
				&context);

#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
#endif//#if defined (DEBUG) || defined (_DEBUG)
			if(!FAILED(hr) && pDevice != NULL)
			{

#if defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM
				context->QueryInterface(__uuidof(ID3D11DeviceContext1),(void**) &pDevContext);
				context->Release();
#endif
				break;//successfully created device, don't need the next try
			}
		}//for (size_t i = 0; i < numTries; ++i)

		if(FAILED(hr)||!pDevice)
		{
			Log("D3D11CreateDevice() failed!Error Code %d",hr);
			pDevice=0;
			SafeRelease(pFactory);
			SafeDelete(pEnum);
			return HQ_FAILED_CREATE_DEVICE;
		}
	}

	DXGI_ADAPTER_DESC l_adapterDesc = pEnum->GetSelectedAdapterDesc();//get adapter desc

	this->InitFeatureCaps();//get feature capacity

	
	//create swapchain
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	memset(&swapchainDesc,0,sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchainDesc.BufferCount=  2;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferDesc.Format=pEnum->selectedMode->Format;
	swapchainDesc.BufferDesc.Width=sWidth;
	swapchainDesc.BufferDesc.Height=sHeight;
	swapchainDesc.BufferDesc.RefreshRate = pEnum->selectedMode->RefreshRate;
	swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	swapchainDesc.OutputWindow=winfo.hwind;
	swapchainDesc.SampleDesc.Count = pEnum->selectedMulSamplesCount;
#else
	memset(&swapchainDesc,0,sizeof(DXGI_SWAP_CHAIN_DESC1));
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.Format=pEnum->selectedMode->Format;
	swapchainDesc.Width=sWidth;
	swapchainDesc.Height=sHeight;
	swapchainDesc.Stereo = false;
#if defined HQ_WIN_STORE_PLATFORM
	swapchainDesc.BufferCount=  2;
	swapchainDesc.Scaling = DXGI_SCALING_NONE; 
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;//windows store restriction
#else
	swapchainDesc.BufferCount=  1;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH; // On phone, only stretch and aspect-ratio stretch scaling are allowed.
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // On phone, no swap effects are supported.
#endif
	swapchainDesc.SampleDesc.Count = pEnum->selectedMulSamplesCount;
#endif

	bool supported = false;
	while(!supported && swapchainDesc.SampleDesc.Count > 1)
	{
		pDevice->CheckMultisampleQualityLevels(
#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
			swapchainDesc.BufferDesc.Format,
#else
			swapchainDesc.Format , 
#endif
			swapchainDesc.SampleDesc.Count,
			&swapchainDesc.SampleDesc.Quality);
		
		if(swapchainDesc.SampleDesc.Quality > 0)
		{
			supported = true;
			swapchainDesc.SampleDesc.Quality--;
		}
		else
			swapchainDesc.SampleDesc.Count--;
	}

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//if(!pEnum->windowed)
	//	swapchainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_NONPREROTATED;
	swapchainDesc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
	swapchainDesc.Windowed=pEnum->windowed;
	
	{
		//get the exact factory that create the device
		if (l_selectedAdapter == NULL)//we create the d3d device from default adapter, that why we need to get the factory of this adapter, not the factory that we create manually before
		{
			IDXGIDevice* idxgidevice = 0;
			pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&idxgidevice);

			IDXGIAdapter * idxgiadapter = 0;
			idxgidevice->GetAdapter(&idxgiadapter);

			idxgiadapter->GetDesc(&l_adapterDesc);//get the real adapter desc

			IDXGIFactory *idxgifactory = 0;
			idxgiadapter->GetParent(__uuidof(IDXGIFactory), (void**)&idxgifactory);

			//use this factory to create a swap chain
			hr=idxgifactory->CreateSwapChain(pDevice , &swapchainDesc , &pSwapChain);

			//release
			idxgifactory->Release();
			idxgiadapter->Release();
			idxgidevice->Release();
		}
		else
			hr=pFactory->CreateSwapChain(pDevice , &swapchainDesc , &pSwapChain);
	}
#else//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	if (l_selectedAdapter == NULL)//we create the d3d device from default adapter, that why we need to get the factory of this adapter, not the factory that we create manually before
	{
		//get exact factory that create the device
		IDXGIDevice1* idxgidevice = 0;
		pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&idxgidevice);

		IDXGIAdapter * idxgiadapter = 0;
		idxgidevice->GetAdapter(&idxgiadapter);

		idxgiadapter->GetDesc(&l_adapterDesc);//get the real adapter desc

		IDXGIFactory2 *idxgifactory = 0;
		idxgiadapter->GetParent(__uuidof(IDXGIFactory2), (void**)&idxgifactory);

		//use this factory to create a swap chain
		hr=idxgifactory->CreateSwapChainForCoreWindow(pDevice, reinterpret_cast<IUnknown*> (winfo.window.Get()), &swapchainDesc, NULL, &pSwapChain);

		//release
		idxgifactory->Release();
		idxgiadapter->Release();
		idxgidevice->Release();
	}
	else
		hr=pFactory->CreateSwapChainForCoreWindow(pDevice , reinterpret_cast<IUnknown*> (winfo.window.Get()), &swapchainDesc, NULL, &pSwapChain);
#endif//#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	if(FAILED(hr)||!pSwapChain)
	{
		Log("CreateSwapChain() failed!Error Code %d",hr);
		SafeRelease(pDevice);
		SafeRelease(pFactory);
		SafeDelete(pEnum);
		return HQ_FAILED_CREATE_DEVICE;
	}
	

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	this->pFactory->MakeWindowAssociation(winfo.hwind , DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
#else
	Windows::Graphics::Display::DisplayProperties::OrientationChanged +=
		ref new Windows::Graphics::Display::DisplayPropertiesEventHandler([this](Platform::Object^ sender)
		{
			this->OnOrientationChanged();
		});

	this->OnOrientationChanged();
#endif
	this->CreateMainRenderTargetView();
	this->CreateMainDepthStencilView();

	
	//bind render targets
	pDevContext->OMSetRenderTargets(1,&pMainRenderTargetView,pMainDepthStencilView);

	
	this->currentVPs[0].x = this->currentVPs[0].y = 0;
	this->d3dViewPort[0].TopLeftX = this->d3dViewPort[0].TopLeftY = 0.0f;
	this->currentVPs[0].width = sWidth;
	this->currentVPs[0].height = sHeight;
	this->d3dViewPort[0].Width = (hq_float32) sWidth;
	this->d3dViewPort[0].Height = (hq_float32)sHeight;

	for (hquint32 i = 0; i < this->maxNumVPs; ++i)
	{
		this->d3dViewPort[i].MinDepth = 0.0f;
		this->d3dViewPort[i].MaxDepth = 1.0f;
	}
	pDevContext->RSSetViewports(1 , &this->d3dViewPort[0]);

	this->textureMan=new HQTextureManagerD3D11(pDevice,pDevContext,logFileStream, this->m_flushLog);
	this->shaderMan=new HQShaderManagerD3D11(pDevice , pDevContext,this->featureLvl , logFileStream ,this->m_flushLog);
	this->vStreamMan = HQ_NEW HQVertexStreamManagerD3D11(pDevice , pDevContext , 
													static_cast<HQShaderManagerD3D11*> (this->shaderMan) , 
													logFileStream , this->m_flushLog);
	this->renderTargetMan = HQ_NEW HQRenderTargetManagerD3D11(pDevice , pDevContext,
		this->pMainRenderTargetView , this->pMainDepthStencilView ,
		static_cast<HQTextureManagerD3D11*> (textureMan) , 
		logFileStream , this->m_flushLog);
	this->stateMan = HQ_NEW HQStateManagerD3D11(pDevice , pDevContext , pEnum->selectedMulSamplesCount > 1 ,logFileStream , this->m_flushLog);

	this->flags |= RUNNING;

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	/*-------override window procedure---------*/
	if(winfo.hwind != NULL)
	{
		g_winProc = (WNDPROC)GetWindowLongA(winfo.hwind , GWL_WNDPROC);
		SetWindowLongA(winfo.hwind , GWL_WNDPROC , (LONG) OverridingWndProc);
	}
#endif

	/*-------get renderer description----------*/
	char *rendererDesc = NULL;
	size_t len = wcstombs(NULL , l_adapterDesc.Description , 0);
	if (len != -1)
	{
		try
		{
			rendererDesc = HQ_NEW char[len + 1];
			wcstombs(rendererDesc , l_adapterDesc.Description , len + 1);
		}
		catch(std::bad_alloc e)
		{
		}
	}
	if (rendererDesc == NULL)
	{
		switch(featureLvl)
		{
		case D3D_FEATURE_LEVEL_11_0:
			Log("Successfully created! Feature level : 11" );
			break;
		case D3D_FEATURE_LEVEL_10_0:
			Log("Successfully created! Feature level : 10" );
			break;
		case D3D_FEATURE_LEVEL_9_3:
			Log("Successfully created! Feature level : 9.3" );
			break;
		}
	}
	else
	{
		switch(featureLvl)
		{
		case D3D_FEATURE_LEVEL_11_0:
			Log("Successfully created! Renderer : %s , Feature level : 11" ,rendererDesc);
			break;
		case D3D_FEATURE_LEVEL_10_0:
			Log("Successfully created! Renderer : %s , Feature level : 10" ,rendererDesc);
			break;
		case D3D_FEATURE_LEVEL_9_3:
			Log("Successfully created! Renderer : %s , Feature level : 9.3" ,rendererDesc);
			break;
		}

		delete[] rendererDesc;
	}


	return HQ_OK;
}

void HQDeviceD3D11::EnableVSync(bool enable)
{
#if defined HQ_WIN_PHONE_PLATFORM
	return;//vsync always on
#else
	if (enable == this->IsVSyncEnabled())
		return;
	if(enable)
	{
		this->flags |= VSYNC_ENABLE;
		pEnum->vsync = 1;
	}
	else
	{
		this->flags &= ~VSYNC_ENABLE;
		pEnum->vsync = 0;
	}
	pEnum->SaveSettingFile(this->settingFileDir);
#endif
}

void HQDeviceD3D11::CreateMainRenderTargetView()
{
	//create render target view
	ID3D11Texture2D *pBackBuffer;
	this->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL,&pMainRenderTargetView);
	pBackBuffer->Release();
}

void HQDeviceD3D11::CreateMainDepthStencilView()
{
	if (pEnum->selectedDepthStencilFmt == DXGI_FORMAT_FORCE_UINT)
		return;
	//create depthstencil target view
	DXGI_SWAP_CHAIN_DESC sDesc;
	pSwapChain->GetDesc(&sDesc);

	//check if this format is supported and multisample type matches backbuffer's
	UINT formatSupport , qualityLevel;
	HRESULT hr = pDevice->CheckFormatSupport(pEnum->selectedDepthStencilFmt , &formatSupport);
	pDevice->CheckMultisampleQualityLevels(pEnum->selectedDepthStencilFmt , sDesc.SampleDesc.Count , &qualityLevel);
	if (qualityLevel > 0)
		qualityLevel--;


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = sDesc.BufferDesc.Width;
	depthStencilDesc.Height = sDesc.BufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	if (FAILED(hr) || (formatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL) == 0 ||
		qualityLevel != sDesc.SampleDesc.Quality)
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	else
		depthStencilDesc.Format = pEnum->selectedDepthStencilFmt;
	depthStencilDesc.SampleDesc = swapchainDesc.SampleDesc; 
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	pDevice->CreateTexture2D(&depthStencilDesc , 0 , &pDepthStencilBuffer);

	pDevice->CreateDepthStencilView(pDepthStencilBuffer , 0 ,&pMainDepthStencilView);
}



//***********************************
//begin render
//***********************************
HQReturnVal HQDeviceD3D11::BeginRender(HQBool isClearPixel, HQBool isClearDepth, HQBool isClearStencil, hquint32 numRTsToClear){
	
	if (this->flags & RENDER_BEGUN)
		return HQ_FAILED_RENDER_ALREADY_BEGUN;
	this->flags |= RENDER_BEGUN;

	return this->Clear(isClearPixel, isClearDepth , isClearStencil, numRTsToClear);
}
//****************************************
//end render
//****************************************
HQReturnVal HQDeviceD3D11::EndRender(){
#if defined DEBUG || defined _DEBUG
	if(!pDevice)
	{
		Log("not init before use!");
		return HQ_DEVICE_NOT_INIT;
	}
#endif

	if ((this->flags & RENDER_BEGUN) == 0)
		return HQ_FAILED_RENDER_NOT_BEGUN;

	this->flags &= ~RENDER_BEGUN;

	return HQ_OK;
}
/*----------------------------------
DisplayBackBuffer()
------------------------------*/
HQReturnVal HQDeviceD3D11::DisplayBackBuffer()
{
#if defined DEBUG || defined _DEBUG
	if(!pDevice)
	{
		Log("not init before use!");
		return HQ_DEVICE_NOT_INIT;
	}
#endif

#if !defined HQ_WIN_PHONE_PLATFORM && !defined HQ_WIN_STORE_PLATFORM
	if (g_firstTimePresent)
		g_firstTimePresent = false;
#endif
	
	HRESULT hr = pSwapChain->Present(((flags & VSYNC_ENABLE) != 0 ), 0 );

#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)

	this->pDevContext->DiscardView(this->pMainRenderTargetView);
	this->pDevContext->DiscardView(this->pMainDepthStencilView);
	
#	ifdef HQ_WIN_STORE_PLATFORM
	
	//re bind render targets since flip_sequenctial swap effect unbound them
	ID3D11RenderTargetView * const * pCurrentRenderTargetViews = static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->GetRenderTargetViews();
	ID3D11DepthStencilView *pCurrentDepthStencilView = static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->GetDepthStencilView();

	this->pDevContext->OMSetRenderTargets(static_cast<HQRenderTargetManagerD3D11*> (
		this->renderTargetMan)->GetNumActiveRenderTargets(), 
		pCurrentRenderTargetViews, 
		pCurrentDepthStencilView);

#	endif//#	ifdef HQ_WIN_STORE_PLATFORM

#endif//#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)

	if (hr == DXGI_ERROR_DEVICE_REMOVED)
	{
		hr = pDevice->GetDeviceRemovedReason();
		Log("Error: D3D11 device removed. Reason: %x", hr);
		return HQ_FAILED;
	}

	return HQ_OK;
}
//***********************************
//clear
//***********************************
HQReturnVal HQDeviceD3D11::Clear(HQBool isClearPixel, HQBool isClearDepth, HQBool isClearStencil, hquint32 numRTsToClear)
{
#if defined DEBUG || defined _DEBUG
	if(!pDevice)
	{
		Log("not init before use!");
		return HQ_DEVICE_NOT_INIT;
	}
#endif

	//clear toàn bộ màn hình
	{
#if 0
		UINT dsflags=0;
		if(isClearDepth)
			dsflags|=D3D11_CLEAR_DEPTH;//1
		if(isClearStencil)
			dsflags|=D3D11_CLEAR_STENCIL;//2
#else
		UINT dsflags = (isClearDepth) | (isClearStencil << 1);
#endif

		if(isClearPixel)
		{
			ID3D11RenderTargetView * const * pCurrentRenderTargetViews = static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->GetRenderTargetViews();
			UINT numViews = static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->GetNumActiveRenderTargets();

			if (numRTsToClear > 0)
				numViews = min(numViews, numRTsToClear);

			for (UINT i = 0 ; i < numViews ; ++i)
			{
				if (pCurrentRenderTargetViews[i] != NULL)
					pDevContext->ClearRenderTargetView(pCurrentRenderTargetViews[i] , this->clearColor);
			}
		}
		if(dsflags)
		{
			ID3D11DepthStencilView *pCurrentDepthStencilView = static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->GetDepthStencilView();
			if (pCurrentDepthStencilView != NULL)
				pDevContext->ClearDepthStencilView(pCurrentDepthStencilView, dsflags , this->clearDepth , this->clearStencil);
		}
	}
	return HQ_OK;
}
//***********************************
//set clear values
//***********************************
void HQDeviceD3D11::SetClearColori(hq_uint32 red,hq_uint32 green,hq_uint32 blue,hq_uint32 alpha){
	hq_float32 rf = 1.0f / 255.0f;

	clearColor.r = red * rf;
	clearColor.g = green * rf;
	clearColor.b = blue * rf;
	clearColor.a = alpha * rf;

	clearColorui = HQColoruiRGBA((hq_ubyte8)red , 
								(hq_ubyte8)green , 
								(hq_ubyte8)blue ,
								(hq_ubyte8)alpha,
								CL_RGBA);

	this->flags |= CLEAR_COLOR_OR_DEPTH_CHANGED;
}
void HQDeviceD3D11::SetClearColorf(hq_float32 red, hq_float32 green, hq_float32 blue, hq_float32 alpha){
	clearColor.r = red ;
	clearColor.g = green ;
	clearColor.b = blue ;
	clearColor.a = alpha ;

	clearColorui = clearColor;

	this->flags |= CLEAR_COLOR_OR_DEPTH_CHANGED;
}
void HQDeviceD3D11::SetClearDepthVal(hq_float32 val){
	if(val > 0.999999f)
		val = 1.0f;
	else if(val < 0.000001f)
		val = 0.0f;
	clearDepth=val;

	this->flags |= CLEAR_COLOR_OR_DEPTH_CHANGED;
}
void HQDeviceD3D11::SetClearStencilVal(hq_uint32 val){
	clearStencil=val & 0xff;
}

void HQDeviceD3D11::GetAllDisplayResolution(HQResolution *resolutionList , hq_uint32& numResolutions)
{
#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	numResolutions = 1;
	if (resolutionList != NULL)
	{	
		resolutionList[0].width = this->sWidth;
		resolutionList[0].height = this->sHeight;
	}
#else
	pEnum->GetAllDisplayResolution(resolutionList , numResolutions);
#endif
}



HQReturnVal HQDeviceD3D11::SetDisplayMode(hq_uint32 width,hq_uint32 height,bool windowed)
{
#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	return HQ_FAILED;
#else
	HQReturnVal re = ChangeDisplayMode(width,height,windowed, true);
	ShowWindow(winfo.hwind, SW_SHOW);
	return re;
#endif
}

#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)

void HQDeviceD3D11::OnOrientationChanged()
{
#if defined HQ_WIN_STORE_PLATFORM
	using namespace Windows::Graphics::Display;

	// Set the proper orientation for the swap chain
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
	switch (DisplayProperties::CurrentOrientation)
	{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		default:
			throw ref new Platform::FailureException();
	}

	this->pSwapChain->SetRotation(rotation);
#endif//#if defined HQ_WIN_STORE_PLATFORM
}

#endif//#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)


HQReturnVal HQDeviceD3D11::OnWindowSizeChanged(hq_uint32 width,hq_uint32 height)
{
	if (!this->IsWindowed())
		return HQ_FAILED;
	HQReturnVal re = ChangeDisplayMode(width,height,this->IsWindowed(), false);

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	ShowWindow(winfo.hwind, SW_SHOW);
#endif
	return re;
}

HQReturnVal HQDeviceD3D11::ChangeDisplayMode(hq_uint32 width,hq_uint32 height,bool windowed, bool resizeWindow)
{
	if(width == sWidth && height == sHeight 
#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
		&& windowed == this->IsWindowed()
#endif
		)
		return HQ_OK;
#if (defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	windowed = true;//this value doesn't relevant in windows store app
#endif

	bool found = pEnum->ChangeSelectedDisplay(width,height,windowed);
	if(!found)
		return HQ_FAILED;
	RECT Rect={0,0,width,height};

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	swapchainDesc.BufferDesc.Width= width;
	swapchainDesc.BufferDesc.Height = height;
	swapchainDesc.Windowed = windowed;
#else
	swapchainDesc.Width= width;
	swapchainDesc.Height = height;
#endif
	

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	RECT rect;
	rect.left = rect.top = 0 ;
	rect.right = width;
	rect.bottom = height;
	if(!windowed)//request fullscreen
	{
		/*
		AdjustWindowRect(&Rect,WS_POPUP,FALSE);
		SetWindowPos(winfo.hwind, HWND_TOP, 0, 0,
			Rect.right-Rect.left, Rect.bottom-Rect.top, SWP_NOZORDER);
		*/

		if(this->IsWindowed())
		{
			SetParent(winfo.hwind,NULL);	
			SetWindowLongPtrW(winfo.hwind,GWL_STYLE,WS_POPUP);
			if (resizeWindow)
				this->pSwapChain->ResizeTarget(pEnum->selectedMode);//resize window

			while (pSwapChain->SetFullscreenState(TRUE , NULL) == DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
			{
				OutputDebugStringA("mode change still in progress\n");
			}
		}

		flags &= (~WINDOWED);
	}
	else
	{
		/*
		AdjustWindowRect(&Rect,winfo.styles,FALSE);
		SetWindowPos(winfo.hwind, HWND_TOP, winfo.x, winfo.y,
			Rect.right-Rect.left, Rect.bottom-Rect.top, SWP_NOZORDER);
		*/
		if(!this->IsWindowed())
		{
			SetParent(winfo.hwind,winfo.hparent);
			SetWindowLongPtrW(winfo.hwind,GWL_STYLE,winfo.styles);
			while (pSwapChain->SetFullscreenState(FALSE , NULL) == DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
			{
				OutputDebugStringA("mode change still in progress\n");
			}
		}

		flags |=WINDOWED;
	}
#endif

	pEnum->windowed = this->IsWindowed();
	pEnum->SaveSettingFile(this->settingFileDir);
	
    this->sWidth = width;
    this->sHeight = height;	

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	if (resizeWindow)
		this->pSwapChain->ResizeTarget(pEnum->selectedMode);//resize window if in windowed mode or screen if in fullscreen mode
#endif
	this->OnResizeBuffer();

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	g_firstTimePresent = true;
#endif

	return HQ_OK;
}

void HQDeviceD3D11::OnResizeBuffer()
{
	//release old render target & depth stencil buffer
	if (static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->IsUsingDefaultFrameBuffer())
		pDevContext->OMSetRenderTargets(0 , 0 , 0);

	SafeRelease(pMainRenderTargetView);
	SafeRelease(pMainDepthStencilView);
	SafeRelease(pDepthStencilBuffer);

	//recreate render target
#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
	HRESULT hr = this->pSwapChain->ResizeBuffers(2, sWidth ,sHeight ,swapchainDesc.BufferDesc.Format ,
							swapchainDesc.Flags);
#else
	HRESULT hr = this->pSwapChain->ResizeBuffers(swapchainDesc.BufferCount, sWidth ,sHeight ,swapchainDesc.Format ,
							swapchainDesc.Flags);
#endif
	
#if defined _DEBUG || defined DEBUG
	if (FAILED(hr))
		this->Log("ResizeBuffers() failed!");
#endif

	this->CreateMainRenderTargetView();
	this->CreateMainDepthStencilView();
	
	static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->SetDefaultRenderTargetView(this->pMainRenderTargetView);
	static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->SetDefaultDepthStencilView(this->pMainDepthStencilView);

	if (static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->IsUsingDefaultFrameBuffer())
		pDevContext->OMSetRenderTargets(1 ,&this->pMainRenderTargetView , this->pMainDepthStencilView);
	
	static_cast<HQRenderTargetManagerD3D11*> (renderTargetMan)->OnBackBufferResized(sWidth, sHeight);

	this->ResetViewports();//reset viewport

	
}

#if !(defined HQ_WIN_PHONE_PLATFORM || defined HQ_WIN_STORE_PLATFORM)
void HQDeviceD3D11::OnWindowActive(bool active)
{
	if (!this->IsWindowed())
	{
		if (active)
		{
			while (pSwapChain->SetFullscreenState(TRUE , NULL) == DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
			{
				OutputDebugStringA("mode change still in progress\n");
			}
			
			g_firstTimePresent = true;
		}
		else
			ShowWindow(winfo.hwind , SW_MINIMIZE);
	}
}
#endif

/*----------------render-------------------*/
void HQDeviceD3D11::SetPrimitiveMode(HQPrimitiveMode primitiveMode) 
{
	this->d3dPrimitiveMode = this->d3dPrimitivelookupTable[primitiveMode];

	pDevContext->IASetPrimitiveTopology(this->d3dPrimitiveMode);
}
HQReturnVal HQDeviceD3D11::Draw(hq_uint32 vertexCount , hq_uint32 firstVertex) 
{
	if ((this->flags & RENDER_BEGUN)== 0)
		return HQ_FAILED_RENDER_NOT_BEGUN;

	this->OnDraw();

	pDevContext->Draw(vertexCount , firstVertex);
	return HQ_OK;
}
HQReturnVal HQDeviceD3D11::DrawPrimitive(hq_uint32 primitiveCount , hq_uint32 firstVertex)
{
	if ((this->flags & RENDER_BEGUN)== 0)
		return HQ_FAILED_RENDER_NOT_BEGUN;
	hquint vertexCount;
	switch (this->d3dPrimitiveMode)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST :
		vertexCount = primitiveCount * 3;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		vertexCount = primitiveCount + 2;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
		vertexCount = primitiveCount ;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
		vertexCount = primitiveCount << 1;//primitiveCount * 2
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
		vertexCount = primitiveCount + 1;
		break;
	default:
		vertexCount = 0;
	}


	this->OnDraw();

	pDevContext->Draw(vertexCount , firstVertex);
	return HQ_OK;
}
HQReturnVal HQDeviceD3D11::DrawIndexed(hq_uint32 numVertices , hq_uint32 indexCount , hq_uint32 firstIndex )
{
	if ((this->flags & RENDER_BEGUN)== 0)
		return HQ_FAILED_RENDER_NOT_BEGUN;


	this->OnDraw();

	pDevContext->DrawIndexed(indexCount , firstIndex , 0);
	return HQ_OK;
}

HQReturnVal HQDeviceD3D11::DrawIndexedPrimitive(hq_uint32 numVertices , hq_uint32 primitiveCount , hq_uint32 firstIndex )
{
	if ((this->flags & RENDER_BEGUN)== 0)
		return HQ_FAILED_RENDER_NOT_BEGUN;
	hquint indexCount;
	switch (this->d3dPrimitiveMode)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST :
		indexCount = primitiveCount * 3;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		indexCount = primitiveCount + 2;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
		indexCount = primitiveCount ;
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
		indexCount = primitiveCount << 1;//primitiveCount * 2
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
		indexCount = primitiveCount + 1;
		break;
	default:
		indexCount = 0;
	}


	this->OnDraw();

	pDevContext->DrawIndexed(indexCount , firstIndex , 0);
	return HQ_OK;
}

HQReturnVal HQDeviceD3D11::DrawInstancedIndirect(HQDrawIndirectArgsBuffer* buffer, hquint32 elementIndex)
{
	HQDrawIndirectBufferD3D11* pD3DBuffer = static_cast<HQDrawIndirectBufferD3D11*> (buffer);

#if FORCE_UNBIND_BOUND_UAV_BUFFERS
	static_cast<HQShaderManagerD3D11*> (shaderMan)->UnbindBufferFromAllUAVSlots(pD3DBuffer);//make sure it will not be bound to any UAV slot
#endif

	this->OnDraw();

	pDevContext->DrawInstancedIndirect(pD3DBuffer->pD3DBuffer, elementIndex * pD3DBuffer->elementSize);

	return HQ_OK;
}

HQReturnVal HQDeviceD3D11::DrawIndexedInstancedIndirect(HQDrawIndexedIndirectArgsBuffer* buffer, hquint32 elementIndex)
{
	HQDrawIndirectBufferD3D11* pD3DBuffer = static_cast<HQDrawIndirectBufferD3D11*> (buffer);

#if FORCE_UNBIND_BOUND_UAV_BUFFERS
	static_cast<HQShaderManagerD3D11*> (shaderMan)->UnbindBufferFromAllUAVSlots(pD3DBuffer);//make sure it will not be bound to any UAV slot
#endif

	this->OnDraw();

	pDevContext->DrawIndexedInstancedIndirect(pD3DBuffer->pD3DBuffer, elementIndex * pD3DBuffer->elementSize);

	return HQ_OK;
}


HQReturnVal HQDeviceD3D11::SetViewport(const HQViewPort &viewport)
{
	HQReturnVal re = HQ_OK;
	UINT width = static_cast<HQRenderTargetManagerD3D11*> (this->renderTargetMan)->GetRTWidth();
	UINT height = static_cast<HQRenderTargetManagerD3D11*> (this->renderTargetMan)->GetRTHeight();
	
	if (viewport.x + viewport.width > width || viewport.y + viewport.height > height)//viewport area is invalid
	{
		this->currentVPs[0].width = width;
		this->currentVPs[0].height = height;
		this->currentVPs[0].x = this->currentVPs[0].y = 0;

		re = HQ_WARNING_VIEWPORT_IS_INVALID;
	}
	else
		this->currentVPs[0] = viewport;
	
	this->d3dViewPort[0].TopLeftX = (hq_float32)this->currentVPs[0].x;
	this->d3dViewPort[0].TopLeftY = (hq_float32)this->currentVPs[0].y;
	this->d3dViewPort[0].Width = (hq_float32)this->currentVPs[0].width;
	this->d3dViewPort[0].Height = (hq_float32)this->currentVPs[0].height;
	
	pDevContext->RSSetViewports(this->numUsedViewports , this->d3dViewPort);

	return re;
}

HQReturnVal HQDeviceD3D11::SetViewports(const HQViewPort * viewports, hquint32 numViewports)
{
	HQReturnVal re = HQ_OK;
	UINT width = static_cast<HQRenderTargetManagerD3D11*> (this->renderTargetMan)->GetRTWidth();
	UINT height = static_cast<HQRenderTargetManagerD3D11*> (this->renderTargetMan)->GetRTHeight();

	this->numUsedViewports = min(this->maxNumVPs, numViewports);

	for (hquint32 i = 0; i < this->numUsedViewports; ++i)
	{
		if (viewports[i].x + viewports[i].width > width || viewports[i].y + viewports[i].height > height)//viewports[i] area is invalid
		{
			this->currentVPs[i].width = width;
			this->currentVPs[i].height = height;
			this->currentVPs[i].x = this->currentVPs[i].y = 0;

			re = HQ_WARNING_VIEWPORT_IS_INVALID;
		}
		else
			this->currentVPs[i] = viewports[i];

		this->d3dViewPort[i].TopLeftX = (hq_float32)this->currentVPs[i].x;
		this->d3dViewPort[i].TopLeftY = (hq_float32)this->currentVPs[i].y;
		this->d3dViewPort[i].Width = (hq_float32)this->currentVPs[i].width;
		this->d3dViewPort[i].Height = (hq_float32)this->currentVPs[i].height;
	}
	pDevContext->RSSetViewports(this->numUsedViewports, this->d3dViewPort);

	return re;
}

HQReturnVal HQDeviceD3D11::ResetViewports()
{
	return SetViewports(this->currentVPs, this->numUsedViewports);
}

HQReturnVal HQDeviceD3D11::DispatchCompute(hquint32 numGroupX, hquint32 numGroupY, hquint32 numGroupZ)
{
	this->OnDispatchCompute();

	pDevContext->Dispatch(numGroupX, numGroupY, numGroupZ);

	return HQ_OK;
}

HQReturnVal HQDeviceD3D11::DispatchComputeIndirect(HQComputeIndirectArgsBuffer* buffer, hquint32 elementIndex)
{
	HQDrawIndirectBufferD3D11* pD3DBuffer = static_cast<HQDrawIndirectBufferD3D11*> (buffer);

#if FORCE_UNBIND_BOUND_UAV_BUFFERS
	static_cast<HQShaderManagerD3D11*> (shaderMan)->UnbindBufferFromAllUAVSlots(pD3DBuffer);//make sure it will not be bound to any UAV slot
#endif

	this->OnDispatchCompute();

	pDevContext->DispatchIndirect(pD3DBuffer->pD3DBuffer, elementIndex * pD3DBuffer->elementSize);

	return HQ_OK;
}


HQReturnVal HQDeviceD3D11::SetUAVForComputeShader(hquint32 slot, ID3D11UnorderedAccessView * pUAV)
{
	DoDeferredResourceBinding(slot, pUAV, CS_UAV_SLOTS_CHANGED, 
		this->pDeferredCSUAVSlots, this->minUsedCSUAVSlot, this->maxUsedCSUAVSlot, this->flags);

	UINT initialCount = -1;
	if (pUAV == NULL)//unbind resource immediately
		pDevContext->CSSetUnorderedAccessViews(slot, 1, &pUAV, &initialCount);

	return HQ_OK;
}

void HQDeviceD3D11::CommitCSUAVSlotsChanges()
{
	if (this->flags & CS_UAV_SLOTS_CHANGED)
	{
		hqint32 numUAVs = this->maxUsedCSUAVSlot - this->minUsedCSUAVSlot + 1;

		if (numUAVs > 0)
		{
			//set UAVs
			this->pDevContext->CSSetUnorderedAccessViews(
				(UINT) this->minUsedCSUAVSlot,
				(UINT)numUAVs,
				this->pDeferredCSUAVSlots + this->minUsedCSUAVSlot,
				this->CSUAVInitialCounts + this->minUsedCSUAVSlot
				);
		}//if (numUAVs > 0)

		this->flags &= ~CS_UAV_SLOTS_CHANGED;
	}
}

HQReturnVal HQDeviceD3D11::SetVSResource(hquint32 slot, ID3D11ShaderResourceView * pSRV)
{
	DoDeferredResourceBinding(slot, pSRV, VS_SRV_SLOTS_CHANGED,
		this->pVSSRVSlots, this->minUsedVSSRVSlot, this->maxUsedVSSRVSlot, this->flags);

	if (pSRV == NULL)//unbind resource immediately
		pDevContext->VSSetShaderResources(slot, 1, &pSRV);

	return HQ_OK;
}
HQReturnVal HQDeviceD3D11::SetGSResource(hquint32 slot, ID3D11ShaderResourceView * pSRV)
{
	DoDeferredResourceBinding(slot, pSRV, GS_SRV_SLOTS_CHANGED,
		this->pGSSRVSlots, this->minUsedGSSRVSlot, this->maxUsedGSSRVSlot, this->flags);

	if (pSRV == NULL)//unbind resource immediately
		pDevContext->GSSetShaderResources(slot, 1, &pSRV);

	return HQ_OK;
}
HQReturnVal HQDeviceD3D11::SetPSResource(hquint32 slot, ID3D11ShaderResourceView * pSRV)
{
	DoDeferredResourceBinding(slot, pSRV, PS_SRV_SLOTS_CHANGED,
		this->pPSSRVSlots, this->minUsedPSSRVSlot, this->maxUsedPSSRVSlot, this->flags);

	if (pSRV == NULL)//unbind resource immediately
		pDevContext->PSSetShaderResources(slot, 1, &pSRV);

	return HQ_OK;
}
HQReturnVal HQDeviceD3D11::SetCSResource(hquint32 slot, ID3D11ShaderResourceView * pSRV)
{
	DoDeferredResourceBinding(slot, pSRV, CS_SRV_SLOTS_CHANGED,
		this->pCSSRVSlots, this->minUsedCSSRVSlot, this->maxUsedCSSRVSlot, this->flags);

	if (pSRV == NULL)//unbind resource immediately
		pDevContext->CSSetShaderResources(slot, 1, &pSRV);

	return HQ_OK;
}

void HQDeviceD3D11::CommitSRVSlotsChanges()
{
	ID3D11ShaderResourceView * nullView = NULL;

	//check for vertex shader's resource slot changes
	if (this->flags & VS_SRV_SLOTS_CHANGED)
	{
		hqint32 numSRVs = this->maxUsedVSSRVSlot - this->minUsedVSSRVSlot + 1;

		if (numSRVs > 0)
		{
			//set SRVs
			this->pDevContext->VSSetShaderResources(this->minUsedVSSRVSlot, numSRVs, this->pVSSRVSlots + this->minUsedVSSRVSlot);
		}//if (numSRVs > 0)

		this->flags &= ~VS_SRV_SLOTS_CHANGED;
	}

	//check for geo shader's resource slot changes
	if (this->flags & GS_SRV_SLOTS_CHANGED)
	{
		hqint32 numSRVs = this->maxUsedGSSRVSlot - this->minUsedGSSRVSlot + 1;

		if (numSRVs > 0)
		{
			//set SRVs
			this->pDevContext->GSSetShaderResources(this->minUsedGSSRVSlot, numSRVs, this->pGSSRVSlots + this->minUsedGSSRVSlot);
		}//if (numSRVs > 0)

		this->flags &= ~GS_SRV_SLOTS_CHANGED;
	}

	//check for pixel shader's resource slot changes
	if (this->flags & PS_SRV_SLOTS_CHANGED)
	{
		hqint32 numSRVs = this->maxUsedPSSRVSlot - this->minUsedPSSRVSlot + 1;

		if (numSRVs > 0)
		{
			//set SRVs
			this->pDevContext->PSSetShaderResources(this->minUsedPSSRVSlot, numSRVs, this->pPSSRVSlots + this->minUsedPSSRVSlot);
		}//if (numSRVs > 0)

		this->flags &= ~PS_SRV_SLOTS_CHANGED;
	}

	//check for compute shader's resource slot changes
	if (this->flags & CS_SRV_SLOTS_CHANGED)
	{
		hqint32 numSRVs = this->maxUsedCSSRVSlot - this->minUsedCSSRVSlot + 1;

		if (numSRVs > 0)
		{
			//set SRVs
			this->pDevContext->CSSetShaderResources(this->minUsedCSSRVSlot, numSRVs, this->pCSSRVSlots + this->minUsedCSSRVSlot);
		}//if (numSRVs > 0)

		this->flags &= ~CS_SRV_SLOTS_CHANGED;
	}
}

void HQDeviceD3D11::OnDraw()
{
	static_cast<HQRenderTargetManagerD3D11*>(renderTargetMan)->OnDrawOrDispatch();//notify render target manager

	static_cast<HQShaderManagerD3D11*> (shaderMan)->NotifyFFRenderIfNeeded();//make changes to FF emulator if needed

	//this->CommitCSUAVSlotsChanges();

	this->CommitSRVSlotsChanges();
	this->CommitIAStreamBufferChanges();

}
void HQDeviceD3D11::OnDispatchCompute()
{
	static_cast<HQRenderTargetManagerD3D11*>(renderTargetMan)->OnDrawOrDispatch();//notify render target manager

	this->CommitCSUAVSlotsChanges();

	this->CommitSRVSlotsChanges();
	//this->CommitIAStreamBufferChanges();
}


HQReturnVal HQDeviceD3D11::SetVertexBuffer(hquint32 slot, ID3D11Buffer *buffer, UINT stride, UINT offset)
{
	this->pIAStreamBufferSlots[slot] = buffer;
	this->IAStreamStrides[slot] = stride;
	this->IAStreamOffsets[slot] = offset;
	if (buffer == NULL)
	{
		if (minUsedIAStreamSlot == slot)
		{
			while (minUsedIAStreamSlot < maxUsedIAStreamSlot && this->pIAStreamBufferSlots[minUsedIAStreamSlot] == NULL)
			{
				minUsedIAStreamSlot++;
			}
		}

		if (maxUsedIAStreamSlot == slot)
		{
			while (maxUsedIAStreamSlot >= minUsedIAStreamSlot && this->pIAStreamBufferSlots[maxUsedIAStreamSlot] == NULL)
			{
				maxUsedIAStreamSlot--;
			}
		}

		UINT zeroStride, zeroOffset = 0;
		//unbind buffer immediately
		this->pDevContext->IASetVertexBuffers(slot, 1, &buffer, &zeroStride, &zeroOffset);

	}//if (buffer == NULL)
	else
	{
		if ((hqint32)slot < minUsedIAStreamSlot)
			minUsedIAStreamSlot = slot;
		if ((hqint32)slot > maxUsedIAStreamSlot)
			maxUsedIAStreamSlot = slot;
	}
	this->flags |= VBUFFER_SLOTS_CHANGED;

	return HQ_OK;
}

void HQDeviceD3D11::CommitIAStreamBufferChanges()
{
	if (this->flags & VBUFFER_SLOTS_CHANGED)
	{
		hqint32 numBuffers = this->maxUsedIAStreamSlot - this->minUsedIAStreamSlot + 1;

		if (numBuffers > 0)
		{
			this->pDevContext->IASetVertexBuffers(
				(UINT)this->minUsedIAStreamSlot,
				(UINT)numBuffers,
				this->pIAStreamBufferSlots + this->minUsedIAStreamSlot,
				this->IAStreamStrides + this->minUsedIAStreamSlot,
				this->IAStreamOffsets + this->minUsedIAStreamSlot);
		}
		else
		{
			ID3D11Buffer *nullBuffer = NULL;
			UINT zeroStride, zeroOffset = 0;
			this->pDevContext->IASetVertexBuffers(
				0,
				0,
				&nullBuffer,
				&zeroStride,
				&zeroOffset);
		}

		this->flags &= ~VBUFFER_SLOTS_CHANGED;
	}
}