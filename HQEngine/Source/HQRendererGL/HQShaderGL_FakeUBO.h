/*
Copyright (C) 2010-2014  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_SHADER_GL_FAKE_UBO_H
#define HQ_SHADER_GL_FAKE_UBO_H

#include "HQShaderGL_Common.h"

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable : 4250 )//dominance inheritance of HQSysMemBuffer
#endif

struct HQFakeUniformBufferGL: public HQSysMemBuffer, public HQUniformBuffer, public HQBaseIDObject
{
	HQFakeUniformBufferGL(HQSysMemBuffer::Listener *listener, hq_uint32 size, bool isDynamic);
	~HQFakeUniformBufferGL();

	hquint32 actualSize;//actual size allocated
	bool isDynamic;

	typedef HQLinkedList<hquint32, HQPoolMemoryManager> BufferSlotList;
	BufferSlotList boundSlots;//list of slots that this buffer bound to
};

#ifdef WIN32
#	pragma warning( pop )
#endif

//for opengl version that doesn't support UBO
class HQBaseShaderManagerGL_FakeUBO : public HQBaseCommonShaderManagerGL, public HQSysMemBuffer::Listener
{

public:
	HQBaseShaderManagerGL_FakeUBO(HQLogStream* logFileStream, const char * logPrefix, bool flushLog);
	~HQBaseShaderManagerGL_FakeUBO();

	HQReturnVal CreateUniformBuffer(void *initData, hq_uint32 size, bool isDynamic, HQUniformBuffer **pBufferIDOut);
	HQReturnVal RemoveUniformBuffer(HQUniformBuffer* bufferID);
	void RemoveAllUniformBuffers();
	HQReturnVal SetUniformBuffer(hq_uint32 slot, HQUniformBuffer* bufferID) __FINAL__;

	HQReturnVal SetUniformBuffer(HQShaderType stage, hq_uint32 slot, HQUniformBuffer* bufferID){
		return this->HQBaseShaderManagerGL_FakeUBO::SetUniformBuffer(slot, bufferID);//shader stage is ignored since opengl used same set of buffer slots for all shader stages
	}

	/*-----unsupported features---------*/
	HQReturnVal CreateBufferUAV(void* initData, hquint32 elementSize, hquint32 numElements, HQBufferUAV** ppBufferOut) { return HQ_FAILED; }

	HQReturnVal CreateComputeIndirectArgs(void* initData, hquint32 numElements, HQComputeIndirectArgsBuffer** ppBufferOut)  { return HQ_FAILED; }

	HQReturnVal CreateDrawIndirectArgs(void* initData, hquint32 numElements, HQDrawIndirectArgsBuffer** ppBufferOut)  { return HQ_FAILED; }

	HQReturnVal CreateDrawIndexedIndirectArgs(void* initData, hquint32 numElements, HQDrawIndexedIndirectArgsBuffer** ppBufferOut)  { return HQ_FAILED; }

	HQReturnVal SetBufferUAVForComputeShader(hquint32 slot, HQBufferUAV * buffer, hquint32 firstElementIdx, hquint32 numElements)  { return HQ_FAILED; }
	HQReturnVal SetBufferUAVForGraphicsShader(hquint32 slot, HQBufferUAV * buffer, hquint32 firstElementIdx, hquint32 numElements) { return HQ_FAILED; }

	HQReturnVal RemoveBufferUAV(HQBufferUAV * buffer) { return HQ_FAILED; }
	void RemoveAllBufferUAVs() {}
protected:
	virtual void BufferChangeEnded(HQSysMemBuffer* pConstBuffer);//implement HQSysMemBuffer::Listener
	//implement HQBaseCommonShaderManagerGL
	virtual HQBaseShaderProgramGL * CreateNewProgramObject();
	virtual void OnProgramCreated(HQBaseShaderProgramGL *program);
	virtual void OnProgramActivated();
	//implement HQBaseShaderManagerGL
	virtual void OnDraw();//this is called before drawing

	HQIDItemManager<HQFakeUniformBufferGL> uniformBuffers;

	struct BufferSlotInfo;
	BufferSlotInfo* uBufferSlots;
};


#endif
