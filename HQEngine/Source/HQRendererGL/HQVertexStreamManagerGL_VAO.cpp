/*
Copyright (C) 2010-2014  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#include "HQDeviceGLPCH.h"
#include "HQVertexStreamManagerGL_VAO.h"

#define VAO_IBO_DIRTY 0x1

/*-------------HQVertexStreamGL_VAO--------------------------------------*/
HQVertexStreamGL_VAO::HQVertexStreamGL_VAO()
: fromWhichBO(0), stride(0)
{

}

inline hquint32 HQVertexStreamGL_VAO::HashCode() const
{
	return fromWhichBO * 29 * 2654435761 + stride;
}

inline bool HQVertexStreamGL_VAO::operator != (const HQVertexStreamGL_VAO &rhs) const
{ 
	return fromWhichBO != rhs.fromWhichBO || stride != rhs.stride; 
}

/*----------HQVertexArrayObjGL::Params--------------*/
HQVertexArrayObjGL::Params::Params(hquint32 _maxNumAttribs, HQVertexInputLayoutGL *_vInputLayout)
: maxNumAttribs(_maxNumAttribs), vInputLayout(_vInputLayout), indexBufferGL(0)
{
	this->streamSources = HQ_NEW HQVertexStreamGL_VAO[maxNumAttribs];
	this->CalculateHashCode();
}

HQVertexArrayObjGL::Params::Params(const Params& src)
: maxNumAttribs(src.maxNumAttribs), vInputLayout(src.vInputLayout), indexBufferGL(src.indexBufferGL), hashCode(src.hashCode)
{
	this->streamSources = HQ_NEW HQVertexStreamGL_VAO[src.maxNumAttribs];
	for (hquint32 i = 0; i < this->maxNumAttribs; ++i)
		this->streamSources[i] = src.streamSources[i];
}


HQVertexArrayObjGL::Params::~Params()
{
	delete[] this->streamSources;
}

hquint32 HQVertexArrayObjGL::Params::CalculateHashCode()
{
#if defined (__LP64__) || defined(_M_X64) || defined(__amd64__)
	this->hashCode = ((hquint64)this->vInputLayout * 2654435761) & 0xffffffff;
#else
	this->hashCode = (hquint32)this->vInputLayout * 2654435761;
#endif

	this->hashCode = this->hashCode * 29 + this->indexBufferGL;

	for (hquint32 i = 0; i < this->maxNumAttribs; ++i)
		this->hashCode = this->hashCode * 29 + this->streamSources[i].HashCode();

	return this->hashCode;
}
bool HQVertexArrayObjGL::Params::Equal(const Params* params2) const
{
	if (this->vInputLayout != params2->vInputLayout
		|| this->indexBufferGL != params2->indexBufferGL)
		return false;
	for (hquint32 i = 0; i < this->maxNumAttribs; ++i)
	{
		if (this->streamSources[i] != params2->streamSources[i])
			return false;
	}
	return true;
}

/*---------------HQVertexArrayObjGL----------------------------*/

HQVertexArrayObjGL::HQVertexArrayObjGL(const Params &_params)
: params(_params), nameGL(0), dirtyFlags(0)
{
	GLuint currentVBO, currentVAO, currentIBO;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&currentVBO);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&currentVAO);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&currentIBO);
	glGenVertexArrays(1, &this->nameGL);
	glBindVertexArray(this->nameGL);

#if 0
	//these are disabled automatically by VAO on creation
	for (hquint32 i = 0; i < params.maxNumAttribs; ++i)
	{
		//disable all attributes just in case
		glDisableVertexAttribArray(i);
	}
#endif

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, params.indexBufferGL);

	for (hquint32 i = 0; i < params.vInputLayout->numAttribs; ++i)
	{
		HQVertexAttribInfoGL &attrib = params.vInputLayout->attribs[i];
		HQVertexStreamGL_VAO &streamSource = params.streamSources[attrib.streamIndex];
		if (streamSource.fromWhichBO != 0)
		{
			this->usedBuffers.Add(streamSource.fromWhichBO, true);//record that this VAO uses this VBO

			glEnableVertexAttribArray(attrib.attribIndex);
			glBindBuffer(GL_ARRAY_BUFFER, streamSource.fromWhichBO);
			glVertexAttribPointer(
				attrib.attribIndex,
				attrib.size,
				attrib.dataType,
				attrib.normalized,
				streamSource.stride,
				attrib.offset
				);
		}
	}


	//re-bind old VAO
	glBindVertexArray(currentVAO);
	//re-bind old IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentIBO);
	//re-bind old VBO
	glBindBuffer(GL_ARRAY_BUFFER, currentVBO);
}
HQVertexArrayObjGL::~HQVertexArrayObjGL()
{
	if (nameGL)
		glDeleteVertexArrays(1, &this->nameGL);
}

bool HQVertexArrayObjGL::IsBufferUsed(const HQSharedPtr<HQBufferGL> &vBuffer)
{
	bool found;
	bool re = this->usedBuffers.GetItem(vBuffer->bufferName, found);
	
	return found;
}

/*-------------HQVertexStreamManagerGL_VAO-----------------*/

HQVertexStreamManagerGL_VAO::HQVertexStreamManagerGL_VAO(hq_uint32 maxVertexAttribs, HQLogStream *logFileStream, bool flushLog)
: HQVertexStreamManagerGL("GL Vertex Stream Manager (VAO supported): ", maxVertexAttribs, logFileStream, flushLog),
m_currentVAOParams(maxVertexAttribs, NULL),
m_currentVAOParamsChanged(false)
{
	glGenVertexArrays(1, &m_defaultVAO_GL);
	glBindVertexArray(m_defaultVAO_GL);
}
HQVertexStreamManagerGL_VAO::~HQVertexStreamManagerGL_VAO()
{
	this->RemoveAllVertexBuffer();
	this->RemoveAllIndexBuffer();
	this->RemoveAllVertexInputLayout();

	glDeleteVertexArrays(1, &m_defaultVAO_GL);
}

inline void HQVertexStreamManagerGL_VAO::BindIndexBuffer(GLuint ibo)
{
	if (m_currentVAO != NULL)
	{
		//avoid affecting the bound VAO
		glBindVertexArray(m_defaultVAO_GL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBindVertexArray(this->m_currentVAO->nameGL);
	}
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

inline void HQVertexStreamManagerGL_VAO::ActiveVAO(const HQSharedPtr<HQVertexArrayObjGL> &vao)
{
	if (this->m_currentVAO != vao)
	{
		this->m_currentVAO = vao;

		if (this->m_currentVAO == NULL)
			glBindVertexArray(m_defaultVAO_GL);
		else
			glBindVertexArray(this->m_currentVAO->nameGL);
	}
}

HQReturnVal HQVertexStreamManagerGL_VAO::SetVertexBuffer(hq_uint32 vertexBufferID, hq_uint32 streamIndex, hq_uint32 stride)
{
	if (streamIndex >= this->maxVertexAttribs)
		return HQ_FAILED;
	HQSharedPtr<HQBufferGL> vBuffer = this->vertexBuffers.GetItemPointer(vertexBufferID);
	HQVertexStreamGL &stream = this->streams[streamIndex];
	if (stream.vertexBuffer != vBuffer && stream.stride != stride)
	{
		/*-----------update VAO params-----------*/
		if (vBuffer == NULL)
			m_currentVAOParams.streamSources[streamIndex].fromWhichBO = 0;
		else
			m_currentVAOParams.streamSources[streamIndex].fromWhichBO = vBuffer->bufferName;
		m_currentVAOParams.streamSources[streamIndex].stride = stride;
		
		m_currentVAOParamsChanged = true;

		stream.vertexBuffer = vBuffer;
		stream.stride = stride;

	}//if (stream.vertexBuffer != vBuffer && stream.stride != stride)

	return HQ_OK;
}

HQReturnVal HQVertexStreamManagerGL_VAO::SetIndexBuffer(hq_uint32 indexBufferID)
{
	HQSharedPtr<HQIndexBufferGL> iBuffer = this->indexBuffers.GetItemPointer(indexBufferID);
	if (this->activeIndexBuffer != iBuffer)
	{
		//update VAO params
		if (iBuffer == NULL)
			m_currentVAOParams.indexBufferGL = 0;
		else
			m_currentVAOParams.indexBufferGL = iBuffer->bufferName;
		m_currentVAOParamsChanged = true;

		this->indexDataType = iBuffer->dataType;
#if 0
		switch (this->indexDataType)
		{
		case GL_UNSIGNED_INT://0x1405
			this->indexShiftFactor = sizeof(unsigned int) >> 1;//2
			break;
		case GL_UNSIGNED_SHORT://0x1403
			this->indexShiftFactor = sizeof(unsigned short) >> 1;//1
		}
#else//optimized version
		this->indexShiftFactor = (this->indexDataType & 0x0000000f) >> 1;//ex : (GL_UNSIGNED_INT & 0xf) >> 1 = (0x1405 & 0xf) >> 1 = 2  
#endif

		this->activeIndexBuffer = iBuffer;
	}

	return HQ_OK;
}

HQReturnVal HQVertexStreamManagerGL_VAO::SetVertexInputLayout(hq_uint32 inputLayoutID)
{

	HQSharedPtr<HQVertexInputLayoutGL> pVLayout = this->inputLayouts.GetItemPointer(inputLayoutID);
	if (this->activeInputLayout != pVLayout)
	{
		/*-----------update VAO params-----------*/
		m_currentVAOParams.vInputLayout = pVLayout.GetRawPointer();

		m_currentVAOParamsChanged = true;

		this->activeInputLayout = pVLayout;
	}

	return HQ_OK;
}

HQReturnVal HQVertexStreamManagerGL_VAO::MapIndexBuffer(hq_uint32 bufferID, HQMapType mapType, void **ppData)
{
	HQSharedPtr<HQIndexBufferGL> pBuffer = this->indexBuffers.GetItemPointer(bufferID);
#if defined _DEBUG || defined DEBUG
	if (pBuffer == NULL)
		return HQ_FAILED;
#endif
	this->BindIndexBuffer(pBuffer->bufferName);

	if (mapType == HQ_MAP_DISCARD)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pBuffer->size, NULL, pBuffer->usage);
	}
#if defined _DEBUG || defined DEBUG
	if (ppData == NULL)
		return HQ_FAILED;
#endif
	*ppData = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	return HQ_OK;
}
HQReturnVal HQVertexStreamManagerGL_VAO::UnmapIndexBuffer(hq_uint32 bufferID)
{
	HQSharedPtr<HQIndexBufferGL> pBuffer = this->indexBuffers.GetItemPointer(bufferID);
#if defined _DEBUG || defined DEBUG
	if (pBuffer == NULL)
		return HQ_FAILED;
#endif
	this->BindIndexBuffer(pBuffer->bufferName);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	return HQ_OK;
}


HQReturnVal HQVertexStreamManagerGL_VAO::UpdateIndexBuffer(hquint32 bufferID, hq_uint32 offset, hq_uint32 size, const void * pData)
{
	HQSharedPtr<HQIndexBufferGL> pBuffer = this->indexBuffers.GetItemPointer(bufferID);
#if defined _DEBUG || defined DEBUG
	if (pBuffer == NULL)
		return HQ_FAILED;
#endif
	hq_uint32 i = offset + size;
	if (i > pBuffer->size)
		return HQ_FAILED_INVALID_SIZE;
	if (i == 0)//update toàn bộ buffer
		size = pBuffer->size;

	this->BindIndexBuffer(pBuffer->bufferName);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, pData);

	return HQ_OK;
}

HQSharedPtr<HQVertexArrayObjGL> HQVertexStreamManagerGL_VAO::GetOrCreateNewVAO()
{
	if (this->m_currentVAOParams.vInputLayout == 0)
		return HQSharedPtr<HQVertexArrayObjGL>::null;

	bool found;
	HQSharedPtr<HQVertexArrayObjGL> vao = m_vertexArrayObjects.GetItem(&m_currentVAOParams, found);
	if (!found)
	{
		//create new VAO
		vao = HQ_NEW HQVertexArrayObjGL(m_currentVAOParams);
		m_vertexArrayObjects.Add(&vao->params, vao);
	}

	return vao;
}

void HQVertexStreamManagerGL_VAO::Commit()//tell vertex stream manager that we are about to draw
{
	if (m_currentVAOParamsChanged)
	{
		m_currentVAOParamsChanged = false;

		m_currentVAOParams.CalculateHashCode();//re-calcualte hash code
		
		//retrieve appropriate VAO
		HQSharedPtr<HQVertexArrayObjGL> &vao = this->GetOrCreateNewVAO();
		//activate VAO
		this->ActiveVAO(vao);
	}
}

HQReturnVal HQVertexStreamManagerGL_VAO::RemoveVertexBuffer(hq_uint32 vertexBufferID)
{
	HQSharedPtr<HQBufferGL> vBuffer = this->vertexBuffers.GetItemPointer(vertexBufferID);

	HQReturnVal re =(HQReturnVal) HQVertexStreamManagerGL::RemoveVertexBuffer(vertexBufferID);

	if (vBuffer.GetRefCount() == 1)//we are holding the last reference
	{
		this->RemoveDependentVAOs(vBuffer);//remove every VAO the uses this buffer
	}

	return re;
}

HQReturnVal HQVertexStreamManagerGL_VAO::RemoveIndexBuffer(hq_uint32 indexBufferID)
{
	HQSharedPtr<HQIndexBufferGL> iBuffer = this->indexBuffers.GetItemPointer(indexBufferID);

	HQReturnVal re = (HQReturnVal)HQVertexStreamManagerGL::RemoveIndexBuffer(indexBufferID);

	if (iBuffer.GetRefCount() == 1)//we are holding the last reference
	{
		this->RemoveDependentVAOs(iBuffer);//remove every VAO the uses this buffer
	}

	return re;
}

HQReturnVal HQVertexStreamManagerGL_VAO::RemoveVertexInputLayout(hq_uint32 inputLayoutID)
{
	HQSharedPtr<HQVertexInputLayoutGL> vLayout = this->inputLayouts.GetItemPointer(inputLayoutID);

	HQReturnVal re = (HQReturnVal)HQVertexStreamManagerGL::RemoveVertexInputLayout(inputLayoutID);

	if (vLayout.GetRefCount() == 1)//we are holding the last reference
	{
		this->RemoveDependentVAOs(vLayout);//remove every VAO the uses this input layout
	}

	return re;
}

void HQVertexStreamManagerGL_VAO::RemoveAllVertexBuffer()
{
	HQItemManager<HQBufferGL>::Iterator ite;
	for (this->vertexBuffers.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		HQSharedPtr<HQBufferGL> vBuffer = ite.GetItemPointer();
		if (vBuffer.GetRefCount() == 2)
		{
			this->RemoveDependentVAOs(vBuffer);//remove every VAO the uses this buffer
		}
	}

	HQVertexStreamManagerGL::RemoveAllVertexBuffer();
}

void HQVertexStreamManagerGL_VAO::RemoveAllIndexBuffer()
{
	HQItemManager<HQIndexBufferGL>::Iterator ite;
	for (this->indexBuffers.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		HQSharedPtr<HQIndexBufferGL> iBuffer = ite.GetItemPointer();
		if (iBuffer.GetRefCount() == 2)
		{
			this->RemoveDependentVAOs(iBuffer);//remove every VAO the uses this buffer
		}
	}

	HQVertexStreamManagerGL::RemoveAllVertexBuffer();
}


void HQVertexStreamManagerGL_VAO::RemoveAllVertexInputLayout()
{
	HQItemManager<HQVertexInputLayoutGL>::Iterator ite;
	for (this->inputLayouts.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		HQSharedPtr<HQVertexInputLayoutGL> vLayout = ite.GetItemPointer();
		if (vLayout.GetRefCount() == 2)
		{
			this->RemoveDependentVAOs(vLayout);//remove every VAO the uses this input layout
		}
	}

	HQVertexStreamManagerGL::RemoveAllVertexInputLayout();
}

void HQVertexStreamManagerGL_VAO::RemoveDependentVAOs(const HQSharedPtr<HQBufferGL> &vBuffer)
{
	//remove every VAO the uses this buffer
	VAOTableType::Iterator ite;
	for (m_vertexArrayObjects.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		if ((*ite)->IsBufferUsed(vBuffer))
		{
			m_vertexArrayObjects.Remove(ite.GetKey());
			//re-search from the start
			m_vertexArrayObjects.GetIterator(ite);
		}
	}
}

void HQVertexStreamManagerGL_VAO::RemoveDependentVAOs(const HQSharedPtr<HQIndexBufferGL> &iBuffer)
{
	//remove every VAO the uses this buffer
	VAOTableType::Iterator ite;
	for (m_vertexArrayObjects.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		if ((*ite)->params.indexBufferGL == iBuffer->bufferName)
		{
			m_vertexArrayObjects.Remove(ite.GetKey());
			//re-search from the start
			m_vertexArrayObjects.GetIterator(ite);
		}
	}
}

void HQVertexStreamManagerGL_VAO::RemoveDependentVAOs(const HQSharedPtr<HQVertexInputLayoutGL> &vLayout)
{
	//remove every VAO the uses this buffer
	VAOTableType::Iterator ite;
	for (m_vertexArrayObjects.GetIterator(ite); !ite.IsAtEnd(); ++ite)
	{
		if ((*ite)->params.vInputLayout == vLayout.GetRawPointer())
		{
			m_vertexArrayObjects.Remove(ite.GetKey());
			//re-search from the start
			m_vertexArrayObjects.GetIterator(ite);
		}
	}
}