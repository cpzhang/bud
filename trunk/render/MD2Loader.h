//==========================================================================
/**
* @file	  : MD2Loader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-14   9:52
* purpose : 
*/
//==========================================================================

#ifndef __MD2Loader_h__
#define __MD2Loader_h__

//
#include "Common.h"

//
namespace Euclid
{
	class MD2Loader
	{
	public:
		struct	Header
		{
			int ident;                  /* magic number: "IDP2" */
			int version;                /* version: must be 8 */

			int skinwidth;              /* texture width */
			int skinheight;             /* texture height */

			int framesize;              /* size in bytes of a frame */

			int num_skins;              /* number of skins */
			int num_vertices;           /* number of vertices per frame */
			int num_st;                 /* number of texture coordinates */
			int num_tris;               /* number of triangles */
			int num_glcmds;             /* number of opengl commands */
			int num_frames;             /* number of frames */

			int offset_skins;           /* offset skin data */
			int offset_st;              /* offset texture coordinate data */
			int offset_tris;            /* offset triangle data */
			int offset_frames;          /* offset frame data */
			int offset_glcmds;          /* offset OpenGL command data */
			int offset_end;             /* offset end of file */
		};

		/* Texture name */
		struct skin
		{
			skin()
			{
				memset(name, 0, 64);
			}
			char name[64];              /* texture file name */
		};

		/* Texture coords */
		struct texCoord
		{
			texCoord()
			{
				s = 0;
				t = 0;
			}
			short s;
			short t;
		};

		/* Triangle info */
		struct triangle
		{
			triangle()
			{
				memset(v, 0, sizeof(unsigned short) * 3);
				memset(st, 0, sizeof(unsigned short) * 3);
			}
			unsigned short v[3];   /* vertex indices of the triangle */
			unsigned short st[3];       /* tex. coord. indices */
		};

		/* Compressed vertex */
		struct vertex
		{
			vertex()
			{
				memset(v, 0, sizeof(unsigned char) * 3);
				normalIndex = 0;
			}
			unsigned char v[3];         /* position */
			unsigned char normalIndex;  /* normal vector index */
		};

		/* Model frame */
		struct frame
		{
			vec3 scale;             /* scale factor */
			vec3 translate;         /* translation vector */
			char name[16];          /* frame name */
			vertex *vs;				/* list of frame's vertices */
		};

		/* GL command packet */
		struct glcmd
		{
			float s;	/* s texture coord. */
			float t;	/* t texture coord. */
			int index;	/* vertex index */
		};

		struct Data
		{
			skin *skins;
			texCoord *texcoords;
			triangle *triangles;
			frame *frames;
			int *glcmds;

			unsigned int tex_id;
		};

		struct MD2File
		{
			MD2File()
			{
				memset(&_header, 0, sizeof(_header));
				memset(&_data, 0, sizeof(_data));
			}
			Header	_header;
			Data	_data;
		};

		// animation
		struct Animation
		{
			int     first_frame;            // first frame of the animation
			int     last_frame;             // number of frames
			int     fps;                    // number of frames per second
		};

		// animation state
		struct AnimationState
		{
			int     startframe;              // first frame
			int     endframe;                // last frame
			int     fps;                     // frame per second for this animation

			float   curr_time;               // current time
			float   old_time;                // old time
			float   interpol;                // percent of interpolation

			int     type;                    // animation type

			int     curr_frame;              // current frame
			int     next_frame;              // next frame
		};

		//
	public:
		MD2Loader();
		~MD2Loader();

		//
	public:
		/**
		* Load an MD2 model from file.
		*
		* Note: MD2 format stores model's data in little-endian ordering.  On
		* big-endian machines, you'll have to perform proper conversions.
		*/
		bool load(const std::string& fileName);

		void preRender();
		void postRender();

		void renderFrame(int& index);

		void destroy();

		void renderInfo();

		//
	private:
		//
		MD2File*					_md2File;
		//
		std::string					_fileName;
		//
		std::string					_texName;
		//
		std::string					_fileBase;
		//
		IRenderSystem*				_renderSystem;
		//
		ITexture*					_tex;
		//
		std::vector<std::string>	_Possible_Extend;
		//
		int							_currentIndex;
	};
}

#endif // __MD2Loader_h__
 
