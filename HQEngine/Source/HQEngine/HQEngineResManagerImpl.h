/*
Copyright (C) 2010-2014  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_ENGINE_RES_MANAGER_IMPL_H
#define HQ_ENGINE_RES_MANAGER_IMPL_H

#include "../HQEngineResManager.h"
#include "../HQLoggableObject.h"
#include "../HQSharedPointer.h"
#include "HQEngineCommonInternal.h"
#include "HQEngineResParserCommon.h"

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable : 4250 )//dominance inheritance of GetName()
#endif

//texture resource
class HQEngineTextureResImpl: public HQNamedGraphicsRelatedObj, public HQEngineTextureResource{
public:
	HQEngineTextureResImpl(const char* name);
	virtual ~HQEngineTextureResImpl();

	void Init(hquint32 textureID, hquint32 renderTargetID = HQ_NULL_ID) {m_textureID = textureID; m_renderTargetID = renderTargetID;}
	hquint32 GetTextureID() const {return m_textureID;}
	hquint32 GetRenderTargetID() const {return m_renderTargetID;}
	virtual hquint32 GetTexture2DSize(hquint32 &width, hquint32 &height) const;
	virtual bool IsRenderTarget() const {return m_renderTargetID != HQ_NULL_ID;}

private:
	hquint32 m_textureID;
	hquint32 m_renderTargetID;
};

//shader resource
class HQEngineShaderResImpl: public HQNamedGraphicsRelatedObj, public HQEngineShaderResource{
public:
	HQEngineShaderResImpl(const char* name);
	virtual ~HQEngineShaderResImpl();

	void Init(HQShaderType type, hquint32 shaderID) {m_shaderID = shaderID; m_shaderType = type;}
	virtual hquint32 GetShaderID() const {return m_shaderID;}
	virtual HQShaderType GetShaderType() const {return m_shaderType;}

private:
	hquint32 m_shaderID;
	HQShaderType m_shaderType;
};


#ifdef WIN32
#	pragma warning( pop )
#endif

enum HQEngineResLoadType {
	HQ_ERLT_STANDARD
};

//resource loading session
class HQEngineResLoadSessionImpl: public HQEngineResLoadSession{
public:
	HQEngineResLoadSessionImpl(HQEngineResParserNode* root);
	~HQEngineResLoadSessionImpl();
	bool HasMoreResources() const;
	const HQEngineResParserNode * CurrentResource();
	const HQEngineResParserNode * NextResource();//advances to next resource item and returns current resource item

	HQEngineResLoadType m_type;
	union {
		struct{
			HQEngineResParserNode * m_root;
			const HQEngineResParserNode* m_resGroup;
			const HQEngineResParserNode* m_resourceItem;
		};
	};
};

//resource manager
class HQEngineResManagerImpl: public HQEngineResManager, public HQLoggableObject {
public:
	HQEngineResManagerImpl(HQLogStream* stream, bool flushLog);
	~HQEngineResManagerImpl();

	virtual HQReturnVal AddResourcesFromFile(const char* fileName);
	virtual HQEngineResLoadSession* BeginAddResourcesFromFile(const char* fileName);
	virtual bool HasMoreResources(HQEngineResLoadSession* session);
	virtual HQReturnVal AddNextResource(HQEngineResLoadSession* session);
	virtual HQReturnVal EndAddResources(HQEngineResLoadSession* session);

	virtual HQReturnVal AddTextureResource(const char *name,
											const char * image_file,
											bool generateMipmap,
											HQTextureType type
											) ;
	virtual HQReturnVal AddCubeTextureResource(const char *name,
											const char * image_files[6],
											bool generateMipmap
											);
	virtual HQReturnVal AddRenderTargetTextureResource(
								  const char *name,
								  hq_uint32 width , hq_uint32 height,
								  bool hasMipmaps,
								  HQRenderTargetFormat format , 
								  HQMultiSampleType multisampleType,
								  HQTextureType textureType);

	virtual HQReturnVal AddShaderResource(
									 const char * name,
									 HQShaderType type,
									 HQShaderCompileMode compileMode,
									 const char * source_file,
									 const HQShaderMacro * pDefines,
									 const char* entryFunctionName);

	virtual HQReturnVal AddShaderResourceFromByteCode(
									 const char *name,
									 HQShaderType type,
									 const char * source_file);

	virtual HQEngineTextureResource * GetTextureResource(const char* name);
	virtual HQEngineShaderResource * GetShaderResource(const char* name);

	const HQSharedPtr<HQEngineTextureResImpl>& GetTextureResourceSharedPtr(const char* name);
	const HQSharedPtr<HQEngineShaderResImpl>& GetShaderResourceSharedPtr(const char* name);


	virtual HQReturnVal RemoveTextureResource(HQEngineTextureResource* res);
	virtual HQReturnVal RemoveShaderResource(HQEngineShaderResource* res);
	virtual void RemoveAllResources();
private:
	HQReturnVal LoadResource(const HQEngineResParserNode* resource);
	HQReturnVal LoadTexture(const HQEngineResParserNode* textureItem, bool renderTarget = false);
	HQReturnVal LoadShader(const HQEngineResParserNode* shaderItem);


	HQClosedStringPrimeHashTable<HQSharedPtr<HQEngineTextureResImpl> > m_textures;
	HQClosedStringPrimeHashTable<HQSharedPtr<HQEngineShaderResImpl> > m_shaders;
};



#endif