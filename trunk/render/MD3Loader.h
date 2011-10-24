//==========================================================================
/**
* @file	  : MD3Loader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-18   9:53
* purpose : 
*/
//==========================================================================

#ifndef __MD3Loader_h__
#define __MD3Loader_h__

#include "Common.h"

namespace Euclid
{
	class MD3Loader
	{
	public:
		struct sHeader 
		{
			sHeader()
				:	indent(0), version(0), flags(0), num_frames(0), num_skins(0),
					num_surfaces(0), num_tags(0), offset_frames(0), offset_tags(0),
					offset_surfaces(0), offset_eof(0)
			{
				memset(name, 0, sizeof(name));
			}

			static const int INDENT;
			int indent;					/* Magic number. As a string of 4 octets, reads "IDP3"; 
										as unsigned little-endian 860898377 (0x33504449); 
										as unsigned big-endian 1229213747 (0x49445033).*/
			static const int VERSION;
			int version;                /* MD3 version number, latest known is 15, but use the constant MD3_VERSION. */

			unsigned char name[64];		/*MD3 name, usually its pathname in the PK3. ASCII character string, NULL-terminated (C-style).*/

			int flags;

			int num_frames;				/*Number of Frame objects, with a maximum of MD3_MAX_FRAMES. Current value of MD3_MAX_FRAMES is 1024.*/

			int num_tags;				/*Number of Tag objects, with a maximum of MD3_MAX_TAGS. Current value of MD3_MAX_TAGS is 16. There is one set of tags per frame so the total number of tags to read is (NUM_TAGS * NUM_FRAMES).*/

			int num_surfaces;			/*Number of Surface objects, with a maximum of MD3_MAX_SURFACES. Current value of MD3_MAX_SURFACES is 32.*/

			int num_skins;				/*Number of Skin objects. I should note that I have not seen an MD3 using this particular field for anything; this appears to be an artifact from the Quake 2 MD2 format. Surface objects have their own Shader field.*/

			int offset_frames;			/*Relative offset from start of MD3 object where Frame objects start. The Frame objects are written sequentially, that is, when you read one Frame object, you do not need to seek() for the next object.*/

			int offset_tags;			/*Relative offset from start of MD3 where Tag objects start. Similarly written sequentially.*/

			int offset_surfaces;		/*Relative offset from start of MD3 where Surface objects start. Again, written sequentially.*/

			int offset_eof;				/*Relative offset from start of MD3 to the end of the MD3 object. Note there is no offset for Skin objects.*/
		};

		//	General properties of a single animation frame.
		struct sFrame 
		{
			sFrame()
				:	radius(0)
			{
				memset(name, 0, sizeof(name));
			}

			Euler::Vector3	min_bounds;			/*First corner of the bounding box.*/

			Euler::Vector3  max_bounds;			/*Second corner of the bounding box.*/

			Euler::Vector3	local_origin;		/*Local origin, usually (0, 0, 0).*/

			float	radius;						/*Ra dius of bounding sphere.*/

			unsigned char name[16];				/*Name of Frame. ASCII character string, NUL-terminated (C-style)*/
		};

		//	An attachment point for another MD3 model.
		struct sTag 
		{
			sTag()
			{
				memset(name, 0, sizeof(name));
			}

			unsigned char		name[64];		/*Name of Tag object. ASCII character string, NUL-terminated (C-style).*/

			Euler::Vector3		origin;			/*Coordinates of Tag object.*/

			Euler::Vector3		axis[3];		/*3x3 rotation matrix associated with the Tag.*/
		};

		//	An animated triangle mesh.
		struct sSurface
		{
			struct sShader
			{
				sShader()
					:	shader_index(0)
				{
					memset(name, 0, sizeof(name));
				}
		
				unsigned char		name[64];		/*Pathname of shader in the PK3. ASCII character string, NUL-terminated (C-style).*/

				int					shader_index;	/*Shader index number. No idea how this is allocated, but presumably in sequential order of definition.*/
			};

			struct sTriangle 
			{
				sTriangle()
				{
					memset(indexes, 0, sizeof(indexes));
				}

				int				indexes[3];		/*List of offset values into the list of Vertex objects that constitute the corners of the Triangle object. Vertex numbers are used instead of actual coordinates, as the coordinates are implicit in the Vertex object. The triangles have clockwise winding.*/
			};

			struct sTexCoord
			{
				sTexCoord()
				{
					memset(st, 0, sizeof(st));
				}

				float			st[2];			/*s and t texture coordinates, normalized to the range [0, 1]. Values outside the range indicate wraparounds/repeats. Unlike UV coordinates, the origin for texture coordinates is located in the upper left corner (similar to the coordinate system used for computer screens) whereas, in UV mapping, it is placed in the lower left corner. As such, the t value must be flipped to correspond with UV coordinates.*/
			};

			struct sVertex 
			{
				sVertex()
				{
					memset(coord, 0, sizeof(coord));
					memset(normal, 0, sizeof(normal));
				}

				short			coord[3];		/*x, y, and z coordinates in left-handed 3-space, scaled down by factor 1.0/64. (Multiply by 1.0/64 to obtain original coordinate value.)*/

				unsigned char	normal[2];		/*Zenith and azimuth angles of normal vector. 255 corresponds to 2 pi.*/
			};

			struct sHeader
			{
				sHeader()
					:	indent(0), flags(0), num_frames(0), num_shaders(0), num_triangles(0), num_vertices(0),
						offset_triangles(0), offset_shaders(0), offset_st(0), offset_vertices(0),
						offset_end(0)
				{
					memset(name, 0, sizeof(name));
				}

				int					indent;			/*Magic number. As a string of 4 octets, reads "IDP3"; as unsigned little-endian 860898377 (0x33504449); as unsigned big-endian 1229213747 (0x49445033).*/

				unsigned char		name[64];		/*Name of Surface object. ASCII character string, NUL-terminated (C-style).*/

				int					flags;

				int					num_frames;		/*Number of animation frames. This should match NUM_FRAMES in the MD3 header.*/

				int					num_shaders;	/*Number of Shader objects defined in this Surface, with a limit of MD3_MAX_SHADERS. Current value of MD3_MAX_SHADERS is 256.*/

				int					num_vertices;	/*Number of Vertex objects defined in this Surface, up to MD3_MAX_VERTS. Current value of MD3_MAX_VERTS is 4096.*/

				int					num_triangles;	/*Number of Triangle objects defined in this Surface, maximum of MD3_MAX_TRIANGLES. Current value of MD3_MAX_TRIANGLES is 8192.*/

				int					offset_triangles;/*Relative offset from SURFACE_START where the list of Triangle objects starts.*/

				int					offset_shaders;	/*Relative offset from SURFACE_START where the list of Shader objects starts.*/

				int					offset_st;		/*Relative offset from SURFACE_START where the list of ST objects (s-t texture coordinates) starts.*/

				int					offset_vertices;/*Relative offset from SURFACE_START where the list of Vertex objects (X-Y-Z-N vertices) starts.*/

				int					offset_end;		/*Relative offset from SURFACE_START to where the Surface object ends.*/
			};

			sHeader*				header;

			std::vector<sShader*>	shaders;	/*List of Shader objects usually starts immediate after the Surface header, but use OFS_SHADERS (or rather, OFS_SHADERS+SURFACE_START for files).*/

			std::vector<sTriangle*> triangles;	/*List of Triangle objects usually starts immediately after the list of Shader objects, but use OFS_TRIANGLES (+ SURFACE_START).*/
			
			std::vector<sTexCoord*> st;			/*List of ST objects usually starts immediately after the list of Triangle objects, but use OFS_ST (+ SURFACE_START).*/

			std::vector<sVertex*>	vertices;	/*List of Vertex objects usually starts immediate after the list of St objects, but use OFS_XYZNORMALS (+ SURFACE_START). The total number of objects is (NUM_FRAMES * NUM_VERTS). One set of NUM_VERTS Vertex objects describes the Surface in one frame of animation; the first NUM_VERTS Vertex objects describes the Surface in the first frame of animation, the second NUM_VERTEX Vertex objects describes the Surface in the second frame of animation, and so forth.*/
		};
	public:
		MD3Loader();
		~MD3Loader();

	public:
		bool load(const std::string& fileName);

		void preRender();

		void postRender();
		
		void renderFrame(int& index);

		//
	private:
		//
		IRenderSystem*				_renderSystem;
		//
		ITexture*					_tex;

		sHeader*				_header;

		std::vector<sFrame*>	_frames;

		std::vector<sTag*>		_tags;

		std::vector<sSurface*>	_surfaces;
	};
}

#endif // __MD3Loader_h__
 
