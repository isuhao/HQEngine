/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef _HQ_RENDER_TARGET_MANAGER_H_
#define _HQ_RENDER_TARGET_MANAGER_H_

#include "HQRendererCoreType.h"

/*-------------------------------------------------------
Important : 
-multisample render target and depth stencil 
buffer may not work together.
-Direct3d 10 & 11 : depth stencil buffer doesn't work 
with cube texture render target yet.It can cause driver 
crash.cube render taret texture can't active as the same time
as 2d render target texture
-------------------------------------------------------*/

class HQRenderTargetManager
{
protected:
	virtual ~HQRenderTargetManager() {};
public:
	///
	///generate full range mipmaps for render target texture {renderTargetTextureID}
	///this operation may do nothing if mipmaps generation for render target texture is not supported
	///this method must not be called when render target is active , or it can make application crash
	///
	virtual HQReturnVal GenerateMipmaps(HQRenderTargetView* renderTargetTextureID) = 0;

	///
	///create render target texture. 
	///{pRenderTargetViewID_Out} - will store ID of newly created render target view. 
	///{pTextureID_Out} - will store ID of texture in texture manager. 
	///{hasMipmaps} - this texture has full range mipmap or not. 
	///Note : 
	///-{arraySize} is only relevant when creating array texture
	///-if {textureType} = HQ_TEXTURE_CUBE , new texture will be created with size {width} x {width}. 
	///-openGL ES 2.0 device always create texture with full range mipmap levels. 
	///-return HQ_FAILED_FORMAT_NOT_SUPPORT if {format} is not supported. 
	///-return HQ_FAILED_MULTISAMPLE_TYPE_NOT_SUPPORT if {multisampleType} is not supported. 
	///
	virtual HQReturnVal CreateRenderTargetTexture(hq_uint32 width , hq_uint32 height,
									  hq_uint32 arraySize,
									  bool hasMipmaps,
									  HQRenderTargetFormat format , 
									  HQMultiSampleType multisampleType,
									  HQTextureType textureType,
									  HQRenderTargetView** pRenderTargetViewID_Out,
									  HQTexture **pTextureID_Out) = 0;
	///
	///create custom depth stencil buffer. 
	///return HQ_FAILED_FORMAT_NOT_SUPPORT if {format} is not supported. 
	///return HQ_FAILED_MULTISAMPLE_TYPE_NOT_SUPPORT if {multisampleType} is not supported. 
	///
	virtual HQReturnVal CreateDepthStencilBuffer(hq_uint32 width , hq_uint32 height,
										HQDepthStencilFormat format,
										HQMultiSampleType multisampleType,
										HQDepthStencilBufferView **pDepthStencilBufferID_Out) = 0;
	
	
	///
	///Create a group of render targets specified in {renderTargetDescs} array. 
	///Depth stencil buffer {depthStencilBufferID} will be used as main depth stencil buffer. 
	///If ID of render target in (ith) element of {renderTargetDescs} array is invalid , render target at (ith) slot will be deactivated. 
	///If {renderTargetDescs} = NULL or {numRenderTargets} = 0 or all render target IDs is not valid , this method will return HQ_FAILED. 
	///Else if {depthStencilBufferID} is not a valid id , no depth stencil buffer will be used. 
	///Depth stencil buffer and render target must have the same multimsample type and width,height of depth stencil buffer must be larger than or equal to render target's. 
	///Render targets must have compatible formats and same width and height. 
	///First render target slot must be a valid render target.Otherwise it can cause undefined behavior.Some render device may switch to default frame buffer. 
	///If current viewport area can't fit in render area ,viewport will be resized to the same size as render target. 
	///return HQ_FAILED if [numRenderTargets} is larger than value retrieved by method GetMaxActiveRenderTargets()
	///
	virtual HQReturnVal CreateRenderTargetGroup(const HQRenderTargetDesc *renderTargetDescs , 
									HQDepthStencilBufferView* depthStencilBufferID,
									hq_uint32 numRenderTargets,//number of render targers
									HQRenderTargetGroup **pRenderTargetGroupID_out
								   )= 0;

	///
	///Create render target group with only one render target. See above method for more details
	///
	HQReturnVal CreateRenderTargetGroup(HQRenderTargetView* renderTargetID,
									HQDepthStencilBufferView* depthStencilBufferID,
									HQRenderTargetGroup **pRenderTargetGroupID_out
									) {
		HQRenderTargetDesc desc (renderTargetID);
		return CreateRenderTargetGroup(&desc, depthStencilBufferID, 1, pRenderTargetGroupID_out);
	}

	///
	///Set the render targets in group {renderTargetGroupID} as main render targets	.
	///Pass NULL to switch to default render target. 
	///Note: D3D11: active render target texture will unset the corresponding texture from every texture slots
	///
	virtual HQReturnVal ActiveRenderTargets(HQRenderTargetGroup* renderTargetGroupID) = 0;

	///
	///Get current render targets group
	///
	virtual HQRenderTargetGroup* GetActiveRenderTargets() = 0;

	///
	///If render target is texture , it also will be removed from texture manager.
	///If there is currently a render target group containing this render target, this render target will not be immediately destroyed
	///
	virtual HQReturnVal RemoveRenderTarget(HQRenderTargetView* renderTargetID) =0;
	virtual void RemoveAllRenderTarget() = 0;
	
	virtual HQReturnVal RemoveDepthStencilBuffer(HQDepthStencilBufferView* depthStencilBufferID) = 0;
	virtual void RemoveAllDepthStencilBuffer() = 0;

	virtual HQReturnVal RemoveRenderTargetGroup(HQRenderTargetGroup* groupID) = 0;
	virtual void RemoveAllRenderTargetGroup() = 0;
};
#endif
