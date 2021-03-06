/*
Copyright (C) 2010-2014  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_SHADER_GL_SHADER_STORAGE_SUPPORTED_H
#define HQ_SHADER_GL_SHADER_STORAGE_SUPPORTED_H

#include "HQShaderGL_UBO.h"
#include "HQCommonGL.h"

#ifdef HQ_GL_UNIFORM_BUFFER_DEFINED
#	ifdef GL_SHADER_STORAGE_BUFFER
#		define HQ_GL_SHADER_STORAGE_BUFFER_DEFINED
#	endif
#endif

#ifdef HQ_GL_SHADER_STORAGE_BUFFER_DEFINED

#ifndef HQ_DEVICE_D3D11_CLASS_FORWARD_DECLARED
#define HQ_DEVICE_D3D11_CLASS_FORWARD_DECLARED
class HQDeviceGL;
#endif

class HQBaseShaderManagerGL_StorageBlockSupprted;

/*----------HQShaderStorageBufferGL--------------------*/
struct HQShaderStorageBufferGL : public HQBufferGL {
	HQShaderStorageBufferGL(hq_uint32 elemSize, hquint32 numElems, GLenum target);
	virtual ~HQShaderStorageBufferGL();

	//implement HQBufferGL
	virtual void Init(const void *initData);

	//implement HQMappableResource
	virtual hquint32 GetSize() const;///mappable size
	virtual HQReturnVal Update(hq_uint32 offset, hq_uint32 size, const void * pData);
	virtual HQReturnVal Unmap();
	virtual HQReturnVal GenericMap(void ** ppData, HQMapType mapType, hquint32 offset, hquint32 size);

	//implement HQGraphicsBufferRawRetrievable
	virtual HQReturnVal TransferData(HQGraphicsBufferRawRetrievable* src, hquint32 destOffset, hquint32 srcOffset, hquint32 size);

	virtual void BindBuffer() = 0;

	HQDeviceGL * pMasterDevice;

};

/*----------------------HQDispatchIndirectBufferGL---------------------------*/
struct HQDispatchIndirectBufferGL: public HQShaderStorageBufferGL{
	HQDispatchIndirectBufferGL(hq_uint32 elemSize, hquint32 numElems);
	~HQDispatchIndirectBufferGL();

	virtual void BindBuffer();
	virtual HQReturnVal CopyContent(void *dest);
};

/*----------------------HQDrawIndirectBufferGL---------------------------*/
struct HQDrawIndirectBufferGL: public HQShaderStorageBufferGL{
	HQDrawIndirectBufferGL(hq_uint32 elemSize, hquint32 numElems);
	~HQDrawIndirectBufferGL();

	virtual void BindBuffer();
	virtual HQReturnVal CopyContent(void *dest);
};

/*----------------------HQGeneralPurposeStorageBufferGL---------------------------*/
struct HQGeneralPurposeStorageBufferGL : public HQShaderStorageBufferGL{
	HQGeneralPurposeStorageBufferGL(hq_uint32 elemSize, hquint32 numElems);
	~HQGeneralPurposeStorageBufferGL();

	virtual void BindBuffer();
	virtual HQReturnVal CopyContent(void *dest);
};

/*------------HQBaseShaderManagerGL_StorageBlockSupprted-------------------*/
class HQBaseShaderManagerGL_StorageBlockSupprted : public HQBaseShaderManagerGL_UBO{
public:
	HQBaseShaderManagerGL_StorageBlockSupprted(HQLogStream* logFileStream, const char * logPrefix, bool flushLog);
	~HQBaseShaderManagerGL_StorageBlockSupprted();

	HQReturnVal CreateBufferUAV(void* initData, hquint32 elementSize, hquint32 numElements, HQBufferUAV** ppBufferOut);

	HQReturnVal CreateComputeIndirectArgs(void* initData, hquint32 numElements, HQComputeIndirectArgsBuffer** ppBufferOut);

	HQReturnVal CreateDrawIndirectArgs(void* initData, hquint32 numElements, HQDrawIndirectArgsBuffer** ppBufferOut);

	HQReturnVal CreateDrawIndexedIndirectArgs(void* initData, hquint32 numElements, HQDrawIndexedIndirectArgsBuffer** ppBufferOut);

	HQReturnVal SetBufferUAVForComputeShader(hquint32 slot, HQBufferUAV * buffer, hquint32 firstElementIdx, hquint32 numElements);
	HQReturnVal SetBufferUAVForGraphicsShader(hquint32 slot, HQBufferUAV * buffer, hquint32 firstElementIdx, hquint32 numElements) 
	{ 
		return this->HQBaseShaderManagerGL_StorageBlockSupprted::SetBufferUAVForComputeShader(slot, buffer, firstElementIdx, numElements);
	}

	HQReturnVal RemoveBufferUAV(HQBufferUAV * buffer);
	void RemoveAllBufferUAVs();

	void BindShaderStorageBuffer(GLuint buffer){
		if (this->boundStorageBuffer != buffer){
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
			this->boundStorageBuffer = buffer;
		}
	}

	GLuint GetBoundShaderStorageBuffer() const { return this->boundStorageBuffer; }

private:
	HQDeviceGL *pMasterDevice;

	HQIDItemManager<HQShaderStorageBufferGL> shaderStorageBuffers;

	GLuint boundStorageBuffer;

	HQSharedPtr<HQBufferGL> *shaderStorageBufferSlots;//binding slots for shader storage buffer
};

#endif//ifdef HQ_GL_SHADER_STORAGE_BUFFER_DEFINED

#endif