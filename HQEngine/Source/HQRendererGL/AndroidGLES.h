/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_GLES_H
#define HQ_GLES_H

#ifndef HQ_OPENGLES
#	define HQ_OPENGLES
#endif

#ifndef GL_GLEXT_PROTOTYPES
#	define GL_GLEXT_PROTOTYPES
#endif

#include <GLES/gl.h>
#include <GLES/glext.h>
#ifndef NO_GLES2_HEADERS
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#define GLEW_OK 0

extern GLboolean GLEW_VERSION_1_1;
extern GLboolean GLEW_VERSION_1_2;
extern GLboolean GLEW_VERSION_1_3;
extern GLboolean GLEW_VERSION_1_4;
extern GLboolean GLEW_VERSION_1_5;
extern GLboolean GLEW_VERSION_2_0;
extern GLboolean GLEW_VERSION_2_1;
extern GLboolean GLEW_VERSION_3_0;
extern GLboolean GLEW_VERSION_3_1;
extern GLboolean GLEW_VERSION_3_2;
extern GLboolean GLEW_VERSION_3_3;
extern GLboolean GLEW_VERSION_4_0;
extern GLboolean GLEW_VERSION_4_1;
extern GLboolean GLEW_VERSION_4_2;
extern GLboolean GLEW_VERSION_4_3;

extern GLboolean GLEW_ARB_multisample;
extern GLboolean GLEW_EXT_texture_filter_anisotropic;
extern GLboolean GLEW_NV_multisample_filter_hint;
extern GLboolean GLEW_EXT_texture_compression_s3tc;
extern GLboolean GLEW_EXT_geometry_shader4;
extern GLboolean GLEW_EXT_framebuffer_object;
extern GLboolean GLEW_ARB_draw_buffers;
extern GLboolean GLEW_ARB_texture_float;
extern GLboolean GLEW_EXT_packed_depth_stencil;
extern GLboolean GLEW_ARB_texture_buffer_object;
extern GLboolean GLEW_EXT_texture_buffer_object;
extern GLboolean GLEW_EXT_texture_integer;
extern GLboolean GLEW_ARB_texture_rg;
extern GLboolean GLEW_NV_gpu_shader4;
extern GLboolean GLEW_EXT_gpu_shader4;
extern GLboolean GLEW_ARB_uniform_buffer_object;

//opengl es extensions
extern GLboolean GLEW_OES_mapbuffer;
extern GLboolean GLEW_OES_compressed_ETC1_RGB8_texture;
extern GLboolean GLEW_IMG_texture_compression_pvrtc;
extern GLboolean GLEW_OES_texture_non_power_of_two;//full support for none power of two texture
extern GLboolean GLEW_EXT_texture_rg;
extern GLboolean GLEW_OES_texture_half_float;
extern GLboolean GLEW_OES_texture_float;

typedef void (GL_APIENTRYP PFNGLBINDRENDERBUFFEROESPROC) (GLenum target, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLDELETERENDERBUFFERSOESPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GL_APIENTRYP PFNGLGENRENDERBUFFERSOESPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEOESPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

typedef void (GL_APIENTRYP PFNGLBINDFRAMEBUFFEROESPROC) (GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSOESPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GL_APIENTRYP PFNGLGENFRAMEBUFFERSOESPROC) (GLsizei n, GLuint* framebuffers);
typedef GLenum (GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSOESPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFEROESPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DOESPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

typedef void (GL_APIENTRYP PFNGLGENERATEMIPMAPOESPROC) (GLenum target);

extern PFNGLGENFRAMEBUFFERSOESPROC android_glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFEROESPROC android_glBindFramebuffer;
extern PFNGLGENRENDERBUFFERSOESPROC android_glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFEROESPROC android_glBindRenderbuffer;
extern PFNGLGENERATEMIPMAPOESPROC android_glGenerateMipmap;
extern PFNGLRENDERBUFFERSTORAGEOESPROC android_glRenderbufferStorage;
extern PFNGLDELETEFRAMEBUFFERSOESPROC android_glDeleteFramebuffers;
extern PFNGLDELETERENDERBUFFERSOESPROC android_glDeleteRenderbuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSOESPROC android_glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERRENDERBUFFEROESPROC android_glFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DOESPROC android_glFramebufferTexture2D;

#ifndef GL_TEXTURE_BUFFER
#define GL_TEXTURE_BUFFER 0x8C2A
#endif

#ifndef GL_TEXTURE_2D_ARRAY
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#endif

#define GL_DEPTH24_STENCIL8_EXT GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT32 0
#define GL_RGBA8 GL_RGBA
#define GL_LUMINANCE8 GL_LUMINANCE
#define GL_ALPHA8 GL_ALPHA
#define GL_LUMINANCE8_ALPHA8 GL_LUMINANCE_ALPHA
#define GL_RGB8 GL_RGB
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT -1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT -1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT -1
#endif

#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_RED_INTEGER 0x8D94
#define GL_RG_INTEGER 0x8228
#define GL_RGBA_INTEGER 0x8D99
#define GL_RED 0x1903
#define GL_RG 0x8227
#define GL_LUMINANCE8UI_EXT 0x8D80
#define GL_LUMINANCE_INTEGER_EXT 0x8D9C
#define GL_RGBA32F 0x8814
#define GL_RGBA16F 0x881A
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_RGBA32I 0x8D82
#define GL_RGBA32UI 0x8D70

#if GL_OES_mapbuffer
#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#define glMapBuffer glMapBufferOES
#define glUnmapBuffer glUnmapBufferOES
#endif

#ifndef GL_READ_ONLY
#define GL_READ_ONLY 0x88B8
#endif
#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 0x88B9
#endif
#ifndef GL_READ_WRITE
#define GL_READ_WRITE 0x88BA
#endif

#ifndef GL_OES_texture_half_float
#	define GL_HALF_FLOAT_OES                0x8D61
#endif

#ifndef GL_EXT_texture_rg
#	define GL_RED_EXT                 0x1903
#	define GL_RG_EXT                  0x8227
#endif

#if GL_OES_framebuffer_object

#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0_EXT GL_COLOR_ATTACHMENT0_OES
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE_OES
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER_OES

#define glGenFramebuffersEXT glGenFramebuffersOES
#define glBindFramebufferEXT glBindFramebufferOES
#define glDeleteFramebuffersEXT glDeleteFramebuffersOES
#define glFramebufferTexture2DEXT glFramebufferTexture2DOES
#define glCheckFramebufferStatusEXT glCheckFramebufferStatusOES
#define glGenRenderbuffersEXT glGenRenderbuffersOES
#define glBindRenderbufferEXT glBindRenderbufferOES
#define glRenderbufferStorageEXT glRenderbufferStorageOES
#define glDeleteRenderbuffersEXT glDeleteRenderbuffersOES

#endif

#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0
#define GL_MAX_TEXTURE_UNITS_ARB GL_MAX_TEXTURE_UNITS

#define GL_CLAMP_TO_BORDER GL_CLAMP_TO_EDGE
#define GL_GEOMETRY_SHADER_EXT 0

#define GL_MAX_SAMPLES 0
#define glRenderbufferStorageMultisample(target , samples, format , width , height) \
			   glRenderbufferStorage(target , format , width , height)




int glewInit();
void nullGlUniformMatrixNonSquare( 	
	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);

#define glUniformMatrix2x3fv nullGlUniformMatrixNonSquare
#define glUniformMatrix2x4fv nullGlUniformMatrixNonSquare
#define glUniformMatrix3x2fv nullGlUniformMatrixNonSquare
#define glUniformMatrix3x4fv nullGlUniformMatrixNonSquare
#define glUniformMatrix4x2fv nullGlUniformMatrixNonSquare
#define glUniformMatrix4x3fv nullGlUniformMatrixNonSquare


#endif
