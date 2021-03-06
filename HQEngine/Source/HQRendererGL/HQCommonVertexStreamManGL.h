/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_COMMON_VERTEX_STREAM_GL_H
#define HQ_COMMON_VERTEX_STREAM_GL_H

#include "../BaseImpl/HQBaseImplCommon.h"
#include "../HQRendererCoreType.h"
#include "../HQSharedPointer.h"
#include "glHeaders.h"
#include "HQCommonGL.h"


class HQVertexStreamManagerGL;


struct HQVertexAttribInfoGL
{
	hquint32 streamIndex;//which stream does this attribute come from
	void* offset;
	GLuint attribIndex;//GL attribute index 
	GLint size;//size of attribute ( one of 1 , 2  , 3 , 4)
	GLenum dataType;
	GLboolean normalized;
};


struct HQVertexInputLayoutGL : public HQVertexLayout, public HQBaseIDObject
{
	HQVertexInputLayoutGL();
	~HQVertexInputLayoutGL();
	
	HQVertexAttribInfoGL *attribs;
	hq_uint32 numAttribs;

	hq_uint32 flags;//flags indicate which attibutes need to be enabled
};
struct HQVertexAttribInfoNodeGL
{
	HQVertexAttribInfoGL *pAttribInfo;
	HQVertexAttribInfoNodeGL *pNext;
};

struct HQVertexStreamGL
{
	HQVertexStreamGL();
	void InsertToStack(HQVertexAttribInfoNodeGL *node);

	HQSharedPtr<HQBufferGL> vertexBuffer;
	hq_uint32 stride;
	HQVertexAttribInfoNodeGL *attribInfoStack;//current vertex attributes that are related to this stream
};


/*---------template class---------*/
template <class VertexStreamManager>
class HQVertexStreamManDelegateGL
{
public:
	//bound vertex buffer {vBuffer} to vertex stream {stream}
	static HQReturnVal SetVertexBuffer(
		VertexStreamManager *manager,
		HQSharedPtr<HQBufferGL>& vBuffer , 
		HQVertexStreamGL & stream , 
		hq_uint32 stride ) ;
	
	//set active vertex layout
	static HQReturnVal SetVertexInputLayout(
		VertexStreamManager *manager,//vertex stream manager
		HQSharedPtr<HQVertexInputLayoutGL>& pVLayout,
		HQSharedPtr<HQVertexInputLayoutGL>& activeInputLayout,//current active layout
		HQVertexStreamGL * streams,//streams
		hquint32 maxVertexAttribs,//max number of vertex attributes
		HQVertexAttribInfoNodeGL *vAttribInfoNodeCache//vertex attributes info cache
		) ;
};



#endif
