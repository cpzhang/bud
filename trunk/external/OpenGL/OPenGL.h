#pragma once
#include <assert.h>
#include <windows.h>
#include <GL/gl.h>
#undef GL_VERSION_1_0
#undef GL_VERSION_1_1
#undef GL_VERSION_1_2
#undef GL_VERSION_1_3
#undef GL_VERSION_1_4
#undef GL_VERSION_1_5
#undef GL_VERSION_2_0
#undef GL_VERSION_2_1
#undef GL_VERSION_3_0
#undef GL_VERSION_3_1
#undef GL_VERSION_3_2
#undef GL_VERSION_3_3
#include <GL/gl3.h>
#include <GL/glext.h>

#ifdef WIN32
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

#ifdef WINDOWS
#include <GL/wglext.h>
#define glGetProcAddress wglGetProcAddress
#define glProcName const char*
#elif defined LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#define glGetProcAddress glXGetProcAddress
#define glProcName const GLubyte*
#endif

#include <stdio.h>

namespace gl {

	class OpenGL {
		// frinds
		friend class Program;
		friend class Shader;
		friend class UniformBlock;
		friend class BlockUniform;
		friend class Uniform;
		friend class IndexBuffer;
		friend class VertexBuffer;
		friend class Texture;
		friend class Texture2D;
		friend class Texture2DArray;
		friend class Texture3D;
		friend class TextureCube;
		friend class Renderbuffer;
		friend class Framebuffer;
		// members
	public:
		bool init(void* window, int majorVersion, int minorVersion, bool vSync, bool stereo);
		void uninit();
		void setVSync(bool enable);
		bool getVSync(void);
		inline bool isInitialized(void) const;
		inline void draw(const int baseVertex, const void* baseIndex, const int numIndices);
		inline void draw(const int* baseVertex, const void** baseIndex, const int* numIndices, int num);
		inline void draw(const int baseVertex, const void* baseIndex, const int numIndices, const int numInstances);
		inline void use(void);
		inline void present(void);
	private:
		bool initFunctions(void);
	public:
		PFNGLENABLEPROC Enable;
		PFNGLDISABLEPROC Disable;
		PFNGLGETINTEGERVPROC GetIntegerv;
		PFNGLCLEARPROC Clear;
		PFNGLCLEARCOLORPROC ClearColor;
		PFNGLCOLORMASKPROC ColorMask;
		PFNGLDEPTHFUNCPROC DepthFunc;
		PFNGLBLENDFUNCPROC BlendFunc;
		PFNGLVIEWPORTPROC Viewport;
		PFNGLFLUSHPROC Flush;
	private:
		// buffer functions
		PFNGLGENBUFFERSPROC GenBuffers;
		PFNGLBINDBUFFERPROC BindBuffer;
		PFNGLDELETEBUFFERSPROC DeleteBuffers;
		PFNGLBUFFERDATAPROC BufferData;
		PFNGLBUFFERSUBDATAPROC BufferSubData;
		PFNGLMAPBUFFERRANGEPROC MapBufferRange;
		PFNGLUNMAPBUFFERPROC UnmapBuffer;
		// vertex array functions
		PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
		PFNGLBINDVERTEXARRAYPROC BindVertexArray;
		PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
		PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
		PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
		//PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
		// texture functions
		PFNGLGENTEXTURESPROC GenTextures;
		PFNGLBINDTEXTUREPROC BindTexture;
		PFNGLDELETETEXTURESPROC DeleteTextures;
		PFNGLACTIVETEXTUREPROC ActiveTexture;
		PFNGLTEXIMAGE2DPROC TexImage2D;
		PFNGLTEXIMAGE3DPROC TexImage3D;
		PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;
		PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
		PFNGLTEXPARAMETERIPROC TexParameteri;
		// framebuffer functions
		PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
		PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
		PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture;
		PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
		PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
		PFNGLDELETERENDERBUFFERSPROC DeleteRenderbuffers;
		PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
		PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
		PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetFramebufferAttachmentParameteriv;
		PFNGLREADBUFFERPROC ReadBuffer;
		PFNGLREADPIXELSPROC ReadPixels;
		PFNGLDRAWBUFFERPROC DrawBuffer;
		PFNGLDRAWBUFFERSPROC DrawBuffers;
		// draw functions
		PFNGLDRAWELEMENTSBASEVERTEXPROC DrawElementsBaseVertex;
		PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC MultiDrawElementsBaseVertex;
		PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC DrawElementsInstancedBaseVertex;
		// uniform functions
		PFNGLBINDBUFFERRANGEPROC BindBufferRange;
		PFNGLBINDBUFFERBASEPROC BindBufferBase;
		PFNGLUNIFORM1IPROC Uniform1i;
		PFNGLUNIFORM2IPROC Uniform2i;
		PFNGLUNIFORM3IPROC Uniform3i;
		PFNGLUNIFORM4IPROC Uniform4i;
		PFNGLUNIFORM1IVPROC Uniform1iv;
		PFNGLUNIFORM2IVPROC Uniform2iv;
		PFNGLUNIFORM3IVPROC Uniform3iv;
		PFNGLUNIFORM4IVPROC Uniform4iv;
		PFNGLUNIFORM1FPROC Uniform1f;
		PFNGLUNIFORM2FPROC Uniform2f;
		PFNGLUNIFORM3FPROC Uniform3f;
		PFNGLUNIFORM4FPROC Uniform4f;
		PFNGLUNIFORM1FVPROC Uniform1fv;
		PFNGLUNIFORM2FVPROC Uniform2fv;
		PFNGLUNIFORM3FVPROC Uniform3fv;
		PFNGLUNIFORM4FVPROC Uniform4fv;
		PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2fv;
		PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3fv;
		PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
		PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
		PFNGLGETUNIFORMBLOCKINDEXPROC GetUniformBlockIndex;
		PFNGLGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
		PFNGLUNIFORMBLOCKBINDINGPROC UniformBlockBinding;
		PFNGLGETACTIVEUNIFORMSIVPROC GetActiveUniformsiv;
		PFNGLGETUNIFORMINDICESPROC GetUniformIndices;
		// shader functions
		PFNGLCREATESHADERPROC CreateShader;
		PFNGLSHADERSOURCEPROC ShaderSource;
		PFNGLCOMPILESHADERPROC CompileShader;
		PFNGLGETSHADERIVPROC GetShaderiv;
		PFNGLISSHADERPROC IsShader;
		PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
		PFNGLDELETESHADERPROC DeleteShader;
		// shader program functions
		PFNGLCREATEPROGRAMPROC CreateProgram;
		PFNGLDELETEPROGRAMPROC DeleteProgram;
		PFNGLLINKPROGRAMPROC LinkProgram;
		PFNGLATTACHSHADERPROC AttachShader;
		PFNGLDETACHSHADERPROC DetachShader;
		PFNGLGETATTACHEDSHADERSPROC GetAttachedShaders;
		PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
		PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
		PFNGLGETPROGRAMIVPROC GetProgramiv;
		PFNGLUSEPROGRAMPROC UseProgram;
		PFNGLBINDFRAGDATALOCATIONPROC BindFragDataLocation;
	private:
		struct {
			int texture;
		} active;
		struct {
			GLuint vertexBuffer;
			GLuint vertexArray;
			GLuint indexBuffer;
			GLuint texture[16];
			GLuint uniformBuffer;
			GLuint program;
			GLuint framebuffer;
			int framebufferAttachments;
			GLuint renderbuffer;
			GLuint readFramebuffer;
			int readLayer;
		} bound;
	private:
#ifdef WINDOWS
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
		struct INTERN {
			HWND hWnd;
			HDC hDC;
			HGLRC hRC;
		} intern;
#endif
		bool initialized;
	};

#include "log.h"
#include "mem.h"

#include "OpenGL.inl"

	extern OpenGL gl;

#include "Shader.h"
#include "Program.h"
#include "Uniform.h"
#include "UniformBlock.h"
#include "BlockUniform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferMemory.h"
#include "IndexBufferMemory.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "Texture3D.h"
#include "TextureCube.h"
#include "Texture2DMemory.h"
#include "Renderbuffer.h"
#include "Framebuffer.h"

#include "BlockUniform.inl"
#include "UniformBlock.inl"
#include "Framebuffer.inl"
#include "IndexBuffer.inl"
#include "VertexBuffer.inl"
#include "Program.inl"
#include "Uniform.inl"

}.
