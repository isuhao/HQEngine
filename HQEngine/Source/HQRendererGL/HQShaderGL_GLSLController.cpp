/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#include "HQDeviceGLPCH.h"
#include "HQShaderGL_GLSLController_inline.h"
#include "HQShaderGL_UBO.h"

#include <string>

#define LOG_SRC_ERR 0

const char semanticKeywords[] =
	"\
#define VPOSITION\n\
#define VCOLOR\n\
#define VNORMAL\n\
#define VTEXCOORD0\n\
#define VTEXCOORD1\n\
#define VTEXCOORD2\n\
#define VTEXCOORD3\n\
#define VTEXCOORD4\n\
#define VTEXCOORD5\n\
#define VTEXCOORD6\n\
#define VTEXCOORD7\n\
#define VTANGENT\n\
#define VBINORMAL\n\
#define VBLENDWEIGHT\n\
#define VBLENDINDICES\n\
#define VPSIZE\n"
;

const char samplerKeywords[] =
"\
#define TEXUNIT0\n\
#define TEXUNIT1\n\
#define TEXUNIT2\n\
#define TEXUNIT3\n\
#define TEXUNIT4\n\
#define TEXUNIT5\n\
#define TEXUNIT6\n\
#define TEXUNIT7\n\
#define TEXUNIT8\n\
#define TEXUNIT9\n\
#define TEXUNIT10\n\
#define TEXUNIT11\n\
#define TEXUNIT12\n\
#define TEXUNIT13\n\
#define TEXUNIT14\n\
#define TEXUNIT15\n\
#define TEXUNIT16\n\
#define TEXUNIT17\n\
#define TEXUNIT18\n\
#define TEXUNIT19\n\
#define TEXUNIT20\n\
#define TEXUNIT21\n\
#define TEXUNIT22\n\
#define TEXUNIT23\n\
#define TEXUNIT24\n\
#define TEXUNIT25\n\
#define TEXUNIT26\n\
#define TEXUNIT27\n\
#define TEXUNIT28\n\
#define TEXUNIT29\n\
#define TEXUNIT30\n\
#define TEXUNIT31\n";


/*-----------HQBaseGLSLShaderController----------------------*/

HQBaseGLSLShaderController::HQBaseGLSLShaderController()
{
	pVParser = new HQVarParserGL(g_pShaderMan);
}
HQBaseGLSLShaderController::~HQBaseGLSLShaderController()
{
	SafeDelete(pVParser);
}

void HQBaseGLSLShaderController::GetPredefineMacroGLSL(std::string & macroDef , const HQShaderMacro * pDefines, std::string &version_string)
{
	if(pDefines == NULL)
		return;
	const HQShaderMacro *pD = pDefines;
	macroDef = "";
	while (pD->name != NULL && pD->definition != NULL)
	{
		if (!strcmp(pD->name , "version") && version_string.size() == 0)
		{
			version_string = "#version ";
            version_string += pD->definition;
            version_string += '\n';
		}
		else
		{
			macroDef += "#define ";
			macroDef += pD->name;
			macroDef += " ";
            macroDef += pD->definition;
            macroDef += '\n';
		}
        pD++;
	}
}

HQReturnVal HQBaseGLSLShaderController::CreateShaderFromStreamGLSL(HQShaderType type,
									 HQDataReaderStream* dataStream,
									 const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
									 HQShaderObjectGL **ppShaderObjectOut)
{
	long size = dataStream->TotalSize();

	char *source = new char[size + 1];

	dataStream->ReadBytes(source,size ,1);
	source[size] = '\0';

	if (dataStream->GetName() != NULL)
			g_pShaderMan->Log("GLSL shader is beging compiled from stream %s ...", dataStream->GetName());

	HQReturnVal re = this->CreateShaderFromMemoryGLSL(type , source , pDefines, ppShaderObjectOut);

	delete[] source;

	if (HQFailed(re))
	{
		if (dataStream->GetName() != NULL)
			g_pShaderMan->Log("GLSL shader compile from stream %s error !", dataStream->GetName());
		else
			g_pShaderMan->Log("GLSL shader compile from stream error !");
	}

	return re;
}


HQReturnVal HQBaseGLSLShaderController::CreateShaderFromMemoryGLSL(HQShaderType type,
									 const char* source,
									 const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
									 HQShaderObjectGL **ppShaderObjectOut)
{
	if(source == NULL)
	{
		return HQ_FAILED;
	}

	GLenum shaderType;
	if(type==HQ_VERTEX_SHADER)
		shaderType = GL_VERTEX_SHADER;
	else if (type==HQ_PIXEL_SHADER)
		shaderType = GL_FRAGMENT_SHADER;
	else if ((GLEW_EXT_geometry_shader4 || GLEW_VERSION_3_2 ) && type==HQ_GEOMETRY_SHADER)
		shaderType = GL_GEOMETRY_SHADER_EXT;
	else
	{
		return HQ_FAILED;
	}

	HQShaderObjectGL *sobject= new HQShaderObjectGL();
	sobject->isGLSL = true;
	bool parseSuccess ;
	std::string processed_src;

	HQLinkedList<HQUniformBlockInfoGL>** ppUBlocksInfo = NULL;
	if (!GLEW_VERSION_3_1 && !GLEW_ARB_uniform_buffer_object)//not support native uniform buffers, so use fake uniform buffers instead
		ppUBlocksInfo = &sobject->pUniformBlocks;

	if(type==HQ_VERTEX_SHADER)
	{
		parseSuccess = pVParser->Parse(source, pDefines, processed_src, ppUBlocksInfo, &sobject->pAttribList, &sobject->pUniformSamplerList);
	}
	else
	{
		parseSuccess = pVParser->Parse(source, pDefines, processed_src, ppUBlocksInfo, NULL, &sobject->pUniformSamplerList);
	}

	if (!parseSuccess)
	{
		delete sobject;
		return HQ_FAILED;
	}

	sobject->shader = glCreateShader(shaderType);

	std::string version_string = "";
	/*------ Remove #version---------*/ 
	{
		size_t pos1 = processed_src.find("#");
		if (pos1 != std::string::npos)
		{
			size_t pos2 = processed_src.find("version", pos1);
			if (pos2 != std::string::npos)
			{
				bool found = true;
				for (size_t i = pos1 + 1; i < pos2; ++i)//make sure only white spaces are between "#" and "version"
				{
					char c = processed_src[i];
					if (c != ' ' && c != '\t' && c != '\r')
					{
						found = false;
						break;
					}
				}
				
				if (found)
				{
					size_t pos3 = processed_src.find("\n", pos2 + 7);
					version_string.assign(source + pos1, pos3 - pos1 + 1); 
					for (size_t i = pos1; i < pos3; ++i)
					{
						processed_src[i] = ' ';//remove from the source
					}
				}

			}//if (pos2 != std::string::npos)
		}//if (pos1 != std::string::npos)
	}

	/*---create macro definition list---------*/
	std::string macroDefList;
	this->GetPredefineMacroGLSL(macroDefList , pDefines, version_string);


	/*--------set shader source---------*/
	int version_number = 0;
	sscanf(version_string.c_str(), "#version %d", &version_number);
	std::string uniform_buffer_extension_line = "";
#ifdef HQ_OPENGLES
	const char prefDefExtVersion[]	= "#define HQEXT_GLSL_ES\n";
#else
	const char prefDefExtVersion[] = "#define HQEXT_GLSL\n";
	if (GLEW_VERSION_3_1 || GLEW_ARB_uniform_buffer_object)
	{
		if (version_number < 140)
			uniform_buffer_extension_line = "#extension GL_ARB_uniform_buffer_object : enable\nlayout(std140) uniform;\n";
	}
#endif
	const GLchar* sourceArray[] = {
		version_string.c_str(),
		uniform_buffer_extension_line.c_str(),
		semanticKeywords,
		samplerKeywords,
		prefDefExtVersion,
		macroDefList.c_str(),
		"#line 0 0\n",
		processed_src.c_str()
	};

	if (type != HQ_VERTEX_SHADER)
		sourceArray[2] = "";//only vertex shader need semantic definitions

	glShaderSource(sobject->shader, 8, (const GLchar**)sourceArray, NULL);
	glCompileShader(sobject->shader);

	GLint compileOK;
	glGetShaderiv(sobject->shader , GL_COMPILE_STATUS , &compileOK);
	
	bool error = (sobject->shader==0 || compileOK == GL_FALSE);
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		glGetShaderiv(sobject->shader, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
		{
#if LOG_SRC_ERR
			int src_length;
			char * glsource;
			glGetShaderiv(sobject->shader, GL_SHADER_SOURCE_LENGTH,&src_length);
			glsource = (char*)malloc(src_length);
			glGetShaderSource(sobject->shader, src_length, NULL, glsource);
#endif
			infoLog = (char *)malloc(infologLength);
	        	glGetShaderInfoLog(sobject->shader, infologLength, &charsWritten, infoLog);
			if (error)
#if LOG_SRC_ERR
				g_pShaderMan->Log("GLSL shader compile error: %s. Source code=<%s>",infoLog, glsource);
#else
				g_pShaderMan->Log("GLSL shader compile error: %s",infoLog);
#endif
			else
				g_pShaderMan->Log("GLSL shader compile info: %s",infoLog);


	        	free(infoLog);
#if LOG_SRC_ERR
			free(glsource);
#endif
	    	}
		if (error)
		{
			delete sobject;
			return HQ_FAILED;
		}
	}
	sobject->type=type;
	
#if 0//can't debug shader if use the following line
	//hide predefine macros
	glShaderSource(sobject->shader, 1 , (const GLchar**)&source, NULL);
#endif
	*ppShaderObjectOut = sobject;

	return HQ_OK;
}

HQReturnVal HQBaseGLSLShaderController::CreateProgramGLSL(
							HQBaseShaderProgramGL *pNewProgramObj,
							  HQSharedPtr<HQShaderObjectGL>& pVShader,
							  HQSharedPtr<HQShaderObjectGL>& pGShader,
							  HQSharedPtr<HQShaderObjectGL>& pFShader,
							  const char** uniformParameterNames,
							  hq_uint32 *pID)
{

	hq_uint32 flags=0;//cờ thể hiện trong program có những loại shader nào

	if(pVShader != NULL)
	{
		if(pVShader->type!=HQ_VERTEX_SHADER//shader có id <vertexShaderID> không phải vertex shader
			|| pVShader->isGLSL == false)//chỉ chấp nhận shader compile từ GL shading language
			return HQ_FAILED_WRONG_SHADER_TYPE;
		flags|=useV;
	}
	if(pFShader != NULL)
	{
		if(pFShader->type!=HQ_PIXEL_SHADER//shader có id <pixelShaderID> không phải pixel shader
			|| pFShader->isGLSL == false)//chỉ chấp nhận shader compile từ GL shading language
			return HQ_FAILED_WRONG_SHADER_TYPE;
		flags|=useF;
	}
	if(pGShader != NULL)
	{
		if(pGShader->type!=HQ_GEOMETRY_SHADER//shader có id <geometryShaderID> không phải geometry shader
			|| pGShader->isGLSL == false)//chỉ chấp nhận shader compile từ GL shading language
			return HQ_FAILED_WRONG_SHADER_TYPE;
		flags|=useG;
	}

	if(flags != useVF && flags != useVGF)//need both valid vertex & pixel shader
		return HQ_FAILED;

	pNewProgramObj->programGLHandle = glCreateProgram();

	if (pNewProgramObj->programGLHandle == 0)
	{
		return HQ_FAILED;
	}

	if(flags & useV)
		glAttachShader(pNewProgramObj->programGLHandle, pVShader->shader);
	if(flags & useG)
		glAttachShader(pNewProgramObj->programGLHandle, pGShader->shader);
	if(flags & useF)
		glAttachShader(pNewProgramObj->programGLHandle, pFShader->shader);

	glLinkProgram(pNewProgramObj->programGLHandle);

	this->BindAttributeLocationGLSL(pNewProgramObj->programGLHandle, *pVShader->pAttribList);

	glLinkProgram(pNewProgramObj->programGLHandle);

	GLint OK;
	glGetProgramiv(pNewProgramObj->programGLHandle, GL_LINK_STATUS, &OK);

	if(OK == GL_FALSE)
	{
		int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;
		glGetProgramiv(pNewProgramObj->programGLHandle, GL_INFO_LOG_LENGTH, &infologLength);
	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(pNewProgramObj->programGLHandle, infologLength, &charsWritten, infoLog);
			g_pShaderMan->Log("GLSL program link error: %s", infoLog);
	        free(infoLog);
	    }
		return HQ_FAILED;
	}

	//create paramters list
	if(uniformParameterNames!=NULL)
	{
		int i=0;
		while(uniformParameterNames[i]!=NULL)
		{
			pNewProgramObj->GetParameter(uniformParameterNames[i]);

			i++;
		}
	}


	//bind sampler unit
	GLuint currentActiveProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentActiveProgram);
	glUseProgram(pNewProgramObj->programGLHandle);

	if(flags & useV)
		this->BindSamplerUnitGLSL(pNewProgramObj,
								  *pVShader->pUniformSamplerList);
	if(flags & useG)
		this->BindSamplerUnitGLSL(pNewProgramObj,
								  *pGShader->pUniformSamplerList);
	if(flags & useF)
		this->BindSamplerUnitGLSL(pNewProgramObj,
								  *pFShader->pUniformSamplerList);
	//bind uniform block
	this->BindUniformBlockGLSL(pNewProgramObj->programGLHandle);

	//back to current program
	glUseProgram(currentActiveProgram);

	//check if this program is valid
	glValidateProgram(pNewProgramObj->programGLHandle);
	glGetProgramiv(pNewProgramObj->programGLHandle, GL_VALIDATE_STATUS, &OK);
	if(OK == GL_FALSE)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		glGetProgramiv(pNewProgramObj->programGLHandle, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(pNewProgramObj->programGLHandle, infologLength, &charsWritten, infoLog);
			g_pShaderMan->Log("GLSL program validate error: %s", infoLog);
			free(infoLog);
		}
		return HQ_FAILED;
	}

	hq_uint32 newProgramID;
	if (!g_pShaderMan->AddItem(pNewProgramObj, &newProgramID))
	{
		return HQ_FAILED_MEM_ALLOC;
	}
	if (pID != NULL)
		*pID = newProgramID;

	return HQ_OK;
}
/*-------------------------*/
void HQBaseGLSLShaderController::BindAttributeLocationGLSL(GLuint program , HQLinkedList<HQShaderAttrib>& attribList)
{
	HQLinkedList<HQShaderAttrib>::Iterator ite;
	attribList.GetIterator(ite);
	for (;!ite.IsAtEnd(); ++ite) {
		if (glGetAttribLocation(program, ite->name.c_str()) != -1) {
			glBindAttribLocation(program, ite->index, ite->name.c_str());
		}
	}
}

void HQBaseGLSLShaderController::BindUniformBlockGLSL(GLuint program)
{
#ifndef HQ_OPENGLES
	if (GLEW_VERSION_3_1 || GLEW_ARB_uniform_buffer_object)
	{
		char blockName[10];
		GLuint index;
		for (int i = 0 ; i < MAX_UNIFORM_BUFFER_SLOTS ; ++i)
		{
			sprintf(blockName , "ubuffer%d" , i );
			index = glGetUniformBlockIndex(program , blockName);
			if (index != GL_INVALID_INDEX)
				glUniformBlockBinding(program , index , i);
		}
	}
#endif
}

void HQBaseGLSLShaderController::BindSamplerUnitGLSL(HQBaseShaderProgramGL* pProgram , HQLinkedList<HQUniformSamplerGL>& samplerList)
{
	HQLinkedList<HQUniformSamplerGL>::Iterator ite;
	samplerList.GetIterator(ite);
	for (;!ite.IsAtEnd(); ++ite) {
		const HQShaderParameterGL* param = pProgram->GetParameter(ite->name.c_str()); 
		if (param != NULL) {
			glUniform1i(param->location , ite->samplerUnit);
		}
	}
}

/*----------HQGLSLShaderController----------------*/

HQReturnVal HQGLSLShaderController::CreateShaderFromStream(HQShaderType type,
										HQDataReaderStream* dataStream,
										const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
										bool isPreCompiled,
										const char* entryFunctionName,
										HQShaderObjectGL **ppShaderObjectOut)
{
	return this->CreateShaderFromStreamGLSL(type,dataStream,pDefines ,ppShaderObjectOut);
}

HQReturnVal HQGLSLShaderController::CreateShaderFromMemory(HQShaderType type,
										  const char* pSourceData,
										  const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
										  bool isPreCompiled,
										  const char* entryFunctionName,
										  HQShaderObjectGL **ppShaderObjectOut)
{
	return this->CreateShaderFromMemoryGLSL(type , pSourceData,pDefines , ppShaderObjectOut);
}

HQReturnVal HQGLSLShaderController::CreateShaderFromStream(HQShaderType type,
								 HQShaderCompileMode compileMode,
								 HQDataReaderStream* dataStream,
								 const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
								 const char* entryFunctionName,
								 HQShaderObjectGL **ppShaderObjectOut)
{
	switch (compileMode)
	{
	case HQ_SCM_GLSL:case HQ_SCM_GLSL_DEBUG:
		return this->CreateShaderFromStreamGLSL(type , dataStream,pDefines,ppShaderObjectOut);

	default:
		return HQ_FAILED_SHADER_SOURCE_IS_NOT_SUPPORTED;
	}
}

HQReturnVal HQGLSLShaderController::CreateShaderFromMemory(HQShaderType type,
								 HQShaderCompileMode compileMode,
								 const char* pSourceData,
								 const HQShaderMacro * pDefines,//pointer đến dãy các shader macro, phần tử cuối phải có cả 2 thành phần <name> và <definition>là NULL để chỉ kết thúc dãy
								 const char* entryFunctionName,
								 HQShaderObjectGL **ppShaderObjectOut)
{
	switch (compileMode)
	{
	case HQ_SCM_GLSL:case HQ_SCM_GLSL_DEBUG:
		return this->CreateShaderFromMemoryGLSL(type , pSourceData,pDefines,ppShaderObjectOut);
	default:
		return HQ_FAILED_SHADER_SOURCE_IS_NOT_SUPPORTED;
	}

}

HQReturnVal HQGLSLShaderController::CreateProgram(
							    HQBaseShaderProgramGL *pNewProgramObj,
								hq_uint32 vertexShaderID,
								hq_uint32 geometryShaderID,
								hq_uint32 pixelShaderID,
								HQSharedPtr<HQShaderObjectGL>& pVShader,
								HQSharedPtr<HQShaderObjectGL>& pGShader,
								HQSharedPtr<HQShaderObjectGL>& pFShader,
								const char** uniformParameterNames,
								hq_uint32 *pID)
{
	if (pNewProgramObj->isGLSL == false)
		return HQ_FAILED;
	hquint32 programID;
	HQReturnVal re = this->CreateProgramGLSL(pNewProgramObj, pVShader, pGShader, pFShader, uniformParameterNames, &programID);
	
	if (pID != NULL)
		*pID = programID;

	//store shaders' IDs
	if (!HQFailed(re))
	{
		HQSharedPtr<HQBaseShaderProgramGL> pProgram = g_pShaderMan->GetItemPointer(programID);
		pProgram->vertexShaderID = vertexShaderID;
		pProgram->geometryShaderID = geometryShaderID;
		pProgram->pixelShaderID = pixelShaderID;
	}

	return re;
}

