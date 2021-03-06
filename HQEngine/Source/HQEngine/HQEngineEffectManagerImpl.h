/*
Copyright (C) 2010-2014  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_ENGINE_EFFECT_MANAGER_IMPL_H
#define HQ_ENGINE_EFFECT_MANAGER_IMPL_H

#include "../HQEngineEffectManager.h"
#include "../HQLinkedList.h"
#include "HQEngineResManagerImpl.h"
#include "HQEngineResParserCommon.h"

//shader program
class HQEngineShaderProgramWrapper: public HQGraphicsRelatedObj {
public:
	struct CreationParams {
		CreationParams() {}
		
		CreationParams& operator = (const CreationParams& params2);

		hquint32 HashCode() const ;
		bool Equal(const CreationParams* params2) const;

		HQSharedPtr<HQEngineShaderResImpl> vertexShader;
		HQSharedPtr<HQEngineShaderResImpl> geometryShader;
		HQSharedPtr<HQEngineShaderResImpl> pixelShader;

	};
	HQEngineShaderProgramWrapper();
	virtual ~HQEngineShaderProgramWrapper();

	HQReturnVal Init(const CreationParams &params);
	virtual HQShaderProgram* GetRawProgram() const {return m_programID;}
	const CreationParams& GetCreationParams() const {return m_creationParams;}
	virtual HQReturnVal Active() ;

private:
	CreationParams m_creationParams;
	HQShaderProgram* m_programID;
};

//blend state
class HQEngineBlendStateWrapper: public HQGraphicsRelatedObj {
public:
	struct CreationParams{
		CreationParams() {isExState = false;}
		
		hquint32 HashCode() const ;
		bool Equal(const CreationParams* params2) const;
		
		CreationParams& operator = (const CreationParams& params2);

		bool isExState;
		HQBlendStateExDesc descEx;
	};

	HQEngineBlendStateWrapper();
	~HQEngineBlendStateWrapper();

	HQReturnVal Init(const CreationParams& creationParams);
	HQReturnVal Active();

	const CreationParams & GetCreationParams() const {return creationParams;}

	CreationParams creationParams;
	hquint32 stateID;
};

//depth stencil state
class HQEngineDSStateWrapper: public HQGraphicsRelatedObj{
public:
	struct CreationParams{
		CreationParams() {isTwoSideState = false;}
		hquint32 HashCode() const ;
		bool Equal(const CreationParams* params2) const;
		
		CreationParams& operator = (const CreationParams& params2);

		bool isTwoSideState;
		
		HQDepthStencilStateDesc desc;
		HQDepthStencilStateTwoSideDesc twoSideDesc;
	};

	HQEngineDSStateWrapper();
	~HQEngineDSStateWrapper();

	HQReturnVal Init(const CreationParams& params);
	HQReturnVal Active();

	const CreationParams & GetCreationParams() const {return creationParams;}

	CreationParams creationParams;
	hquint32 stateID;
};

//sampler state
class HQEngineSamplerStateWrapper: public HQGraphicsRelatedObj{
public:
	struct CreationParams: public HQSamplerStateDesc{
		CreationParams(): HQSamplerStateDesc() {}
		hquint32 HashCode() const ;
		bool Equal(const CreationParams* params2) const;
		CreationParams& operator = (const CreationParams& params2);
	};

	HQEngineSamplerStateWrapper();
	~HQEngineSamplerStateWrapper();

	HQReturnVal Init(const CreationParams& _desc);
	HQReturnVal Apply(hquint32 index);

	const CreationParams & GetCreationParams() const {return desc;}

	CreationParams desc;
	hquint32 stateID;
};

/*--------------texture units-----------------------*/
//generic texture unit: canbe normal unit or UAV unit
struct HQEngineBaseTextureUnit{
	virtual ~HQEngineBaseTextureUnit() {}

	hquint32 unitIndex;//there many be different between D3D and GL. D3D: index = shaderStage bitwise or-ed with texture stage index
	HQSharedPtr<HQEngineTextureResImpl> texture;//texture
};

//texture sampler unit
struct HQEngineTextureUnit : public HQEngineBaseTextureUnit {
	
	HQSharedPtr<HQEngineSamplerStateWrapper> samplerState;// sampling state
};

//texture UAV unit
struct HQEngineTextureUAVUnit : public HQEngineBaseTextureUnit {

	hquint32 mipLevel;
	bool readable;
};

/*------------buffer slot---------------*/
struct HQEngineBaseShaderBufferSlot {
	hquint32 slotIndex;
	HQSharedPtr<HQEngineShaderBufferResImpl> buffer;
};

//UAV buffer slot
struct HQEngineShaderBufferUAVSlot : public HQEngineBaseShaderBufferSlot {
	hquint32 firstElement;
	hquint32 numElements;
};


/*----------depth stencil buffer-----------*/
class HQEngineDSBufferWrapper: public HQGraphicsRelatedObj {
public:
	struct CreationParams{
		CreationParams() {}
		hquint32 HashCode() const;
		bool Equal(const CreationParams* params2) const;
		CreationParams& operator = (const CreationParams& params2);

		HQDepthStencilFormat format;
		hquint32 width, height;
	};

	HQEngineDSBufferWrapper();
	~HQEngineDSBufferWrapper();

	HQReturnVal Init(const CreationParams &params);
	const CreationParams & GetCreationParams() const {return creationParams;}

	CreationParams creationParams;	
	HQDepthStencilBufferView* bufferID;
};

//render target
struct HQEngineRenderTargetWrapper {
	HQEngineRenderTargetWrapper() : cubeFace(HQ_CTF_POS_X) {}

	hquint32 HashCode() const;
	bool Equal(const HQEngineRenderTargetWrapper& rt2) const;

	HQSharedPtr<HQEngineTextureResImpl> outputTexture;
	union {
		hquint32 cubeFace;
		hquint32 arraySlice;
	};
};

//render targets group
class HQEngineRTGroupWrapper: public HQGraphicsRelatedObj {
public:
	struct CreationParams {
		CreationParams();
		~CreationParams();
		hquint32 HashCode() const;
		bool Equal(const CreationParams* params2) const;

		HQSharedArrayPtr<HQEngineRenderTargetWrapper> outputs;
		hquint32 numOutputs;
		HQSharedPtr<HQEngineDSBufferWrapper> dsBuffer;//depth stencil buffer
	};

	HQEngineRTGroupWrapper();
	~HQEngineRTGroupWrapper();

	HQReturnVal Init(const CreationParams &params);
	const CreationParams & GetCreationParams() const {return creationParams;}

	HQReturnVal Active();

	HQRenderTargetGroup* groupID;
	CreationParams creationParams;
};

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable : 4250 )//dominance inheritance of GetName()
#endif


/*----------rendering pass-------------*/
//platform specific controller
class HQEngineRenderPassPlatformController;
//base rendering pass class
struct HQEngineBaseRenderPassImpl : public HQNamedGraphicsRelatedObj, public HQEngineRenderPass {

	HQEngineBaseRenderPassImpl(const char *name, HQEngineRenderPassPlatformController* platformController);
	virtual ~HQEngineBaseRenderPassImpl();

	virtual HQReturnVal Apply();

	void AddTextureUnit(const HQEngineTextureUnit& texunit);
	void AddTextureUAVUnit(const HQEngineTextureUAVUnit& texunit);
	void AddBufferUAVSlot(const HQEngineShaderBufferUAVSlot& bufferSlot);
	virtual bool Validate(HQLoggableObject *logger);

	HQLinkedList<HQEngineTextureUAVUnit > textureUAVUnits;//used UAV texture slots
	HQLinkedList<HQEngineShaderBufferUAVSlot > bufferUAVSlots;//used UAV buffer slots

	HQLinkedList<HQEngineTextureUnit > textureUnits; //controlled texture units in this pass
protected:
	void ApplyTextureStates();

	HQEngineRenderPassPlatformController* platformController;
};

//normal rendering pipeline pass
struct HQEngineGraphicsPassImpl;
//compute pass
struct HQEngineComputePassImpl;

//rendering effect
class HQEngineRenderEffectImpl: public HQNamedGraphicsRelatedObj, public HQEngineRenderEffect {
public:
	HQEngineRenderEffectImpl(const char* name, 
		HQClosedStringPrimeHashTable<HQSharedPtr<HQEngineBaseRenderPassImpl> >& passes);
	virtual ~HQEngineRenderEffectImpl() ;
	virtual hquint32 GetNumPasses() const {return m_numPasses;}
	virtual HQEngineRenderPass* GetPassByName(const char* name);
	virtual hquint32 GetPassIndexByName(const char* name);
	virtual HQEngineRenderPass* GetPass(hquint32 index) {return m_passes[index].GetRawPointer();}
private:
	typedef HQClosedStringPrimeHashTable<hquint32> PassIndexMapTable;
	PassIndexMapTable m_passIdxMap;//render pass index mapping table
	HQSharedPtr<HQEngineBaseRenderPassImpl> * m_passes;//render passes
	hquint32 m_numPasses;
};


#ifdef WIN32
#	pragma warning( pop )
#endif

//effect loading session
enum HQEngineEffectLoadType {
	HQ_EELT_STANDARD
};

class HQEngineEffectLoadSessionImpl: public HQEngineEffectLoadSession{
public:
	HQEngineEffectLoadSessionImpl(HQEngineEffectParserNode* root);
	~HQEngineEffectLoadSessionImpl();
	bool HasMoreEffects() const;
	const HQEngineEffectParserNode * CurrentEffect();
	const HQEngineEffectParserNode * NextEffect();//advances to next effect item and returns current effect item

	const HQEngineEffectLoadType m_type;
	union {
		struct{
			HQEngineEffectParserNode * m_root;
			const HQEngineEffectParserNode* m_effectGroup;
			const HQEngineEffectParserNode* m_effectItem;
		};
	};
};

//effect manager
class HQEngineEffectManagerImpl: public  HQEngineEffectManager, public HQLoggableObject
{
public:
	HQEngineEffectManagerImpl(HQLogStream* stream, bool flushLog);
	virtual ~HQEngineEffectManagerImpl() ;

	virtual void SetSuffix(const char* suffix);

	virtual HQReturnVal AddEffectsFromFile(const char* fileName);
	virtual HQEngineEffectLoadSession* BeginAddEffectsFromFile(const char* fileName);
	virtual bool HasMoreEffects(HQEngineEffectLoadSession* session);
	virtual HQReturnVal AddNextEffect(HQEngineEffectLoadSession* session);
	virtual HQReturnVal EndAddEffects(HQEngineEffectLoadSession* session);///for releasing loading session

	virtual HQReturnVal SetTexture(hq_uint32 slot, HQEngineTextureResource* texture);
	virtual HQReturnVal SetTextureForPixelShader(hq_uint32 slot, HQEngineTextureResource* texture);

	virtual HQEngineRenderEffect * GetEffect(const char *name) ;
	virtual HQReturnVal RemoveEffect(HQEngineRenderEffect *effect) ;
	virtual void RemoveAllEffects() ;

private:
	HQReturnVal LoadEffect(const HQEngineEffectParserNode * effectItem);
	HQReturnVal LoadPass(const HQEngineEffectParserNode* passItem, HQEngineGraphicsPassImpl *newPass);
	HQReturnVal LoadComputePass(const HQEngineEffectParserNode* passItem, HQEngineComputePassImpl *newPass);
	HQReturnVal ParseStencilState(const HQEngineEffectParserNode *stencilElem, HQEngineDSStateWrapper::CreationParams &params);
	HQReturnVal ParseBlendState(const HQEngineEffectParserNode* blendElem, HQEngineBlendStateWrapper::CreationParams &params);
	HQReturnVal ParseSamplerState(const HQEngineEffectParserNode* textureElem, HQEngineSamplerStateWrapper::CreationParams &params);
	HQReturnVal ParseDepthStencilBufferFmt(const HQEngineEffectParserNode* dsBufElem, HQEngineDSBufferWrapper::CreationParams &params);
	HQReturnVal ParseRTGroup(const HQEngineEffectParserNode* rtGroupElem, HQEngineRTGroupWrapper::CreationParams &params);
	HQReturnVal ParseTextureUnit(const HQEngineEffectParserNode* textureUnitElem, HQEngineTextureUnit& textureUnit);
	HQReturnVal ParseTextureUAVUnit(const HQEngineEffectParserNode* textureUnitElem, HQEngineTextureUAVUnit& texUAVUnit);
	HQReturnVal ParseShaderBufferUAVUnit(const HQEngineEffectParserNode* bufferSlotElem, HQEngineShaderBufferUAVSlot& bufferUAVUnit);

	//these methods will create new or return existing object
	HQSharedPtr<HQEngineRTGroupWrapper> CreateOrGetRTGroup(const HQEngineRTGroupWrapper::CreationParams& params);
	HQSharedPtr<HQEngineDSBufferWrapper> CreateOrGetDSBuffer(const HQEngineDSBufferWrapper::CreationParams& params);
	HQSharedPtr<HQEngineShaderProgramWrapper> CreateOrGetShaderProgram(const HQEngineShaderProgramWrapper::CreationParams& params);
	HQSharedPtr<HQEngineBlendStateWrapper> CreateOrGetBlendState(const HQEngineBlendStateWrapper::CreationParams& params);
	HQSharedPtr<HQEngineDSStateWrapper> CreateOrGetDSState(const HQEngineDSStateWrapper::CreationParams& params);
	HQSharedPtr<HQEngineSamplerStateWrapper> CreateOrGetSamplerState(const HQEngineSamplerStateWrapper::CreationParams& params);

	typedef HQClosedPtrKeyHashTable<const HQEngineShaderProgramWrapper::CreationParams*, HQSharedPtr<HQEngineShaderProgramWrapper> > ProgramTable;
	typedef HQClosedPtrKeyHashTable<const HQEngineRTGroupWrapper::CreationParams*, HQSharedPtr<HQEngineRTGroupWrapper> > RTGroupTable;
	typedef HQClosedPtrKeyHashTable<const HQEngineDSBufferWrapper::CreationParams*, HQSharedPtr<HQEngineDSBufferWrapper> > DSBufferTable;
	typedef HQClosedPtrKeyHashTable<const HQEngineBlendStateWrapper::CreationParams*, HQSharedPtr<HQEngineBlendStateWrapper> > BlendStateTable;
	typedef HQClosedPtrKeyHashTable<const HQEngineDSStateWrapper::CreationParams*, HQSharedPtr<HQEngineDSStateWrapper> > DSStateTable;
	typedef HQClosedPtrKeyHashTable<const HQEngineSamplerStateWrapper::CreationParams*, HQSharedPtr<HQEngineSamplerStateWrapper> > SamplerStateTable;
	typedef HQClosedStringPrimeHashTable<HQSharedPtr<HQEngineRenderEffectImpl> > EffectTable;

	HQRenderDevice *m_pRDevice;

	ProgramTable m_shaderPrograms;//shader programs
	RTGroupTable m_renderTargetGroups;//render targets. 
	DSBufferTable m_dsBuffers;//depth stencil buffers
	BlendStateTable m_blendStates;//blend states
	DSStateTable m_dsStates;//depth stencil states
	SamplerStateTable m_samplerStates;//sampler states

	HQEngineRenderPassPlatformController * m_platformController;

	EffectTable m_effects;//rendering effects

	HQClosedStringPrimeHashTable<HQCullMode> m_cullModeMap;
	HQClosedStringPrimeHashTable<HQDepthMode> m_depthModeMap;
	HQClosedStringPrimeHashTable<HQStencilOp> m_stencilOpMap;
	HQClosedStringPrimeHashTable<HQStencilFunc> m_stencilFuncMap;
	HQClosedStringPrimeHashTable<HQBlendFactor> m_blendFactorMap;
	HQClosedStringPrimeHashTable<HQBlendOp> m_blendOpMap;
	HQClosedStringPrimeHashTable<HQTexAddressMode> m_taddrModeMap;
	HQClosedStringPrimeHashTable<HQFilterMode> m_filterModeMap;
	HQClosedStringPrimeHashTable<HQDepthStencilFormat> m_dsFmtMap;

	std::string m_suffix;

	bool m_isGL;
};

#endif