//==========================================================================
/**
* @file	  : EntityYZMaterial.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-23   18:02
* purpose : 
*/
//==========================================================================

#ifndef __EntityYZMaterial_h__
#define __EntityYZMaterial_h__

#include "Common.h"
#include "KeyFrames.h"
#include "Color.h"
#include "Appearance.h"
#include "ITexture.h"

// #	pragma pack( push, packing )
// #	pragma pack( 1 )

namespace Euclid
{
	class EntityYZMaterialLayer
	{
	public:
		EntityYZMaterialLayer()
			:	_texture(NULL), _flowedU(0.0f), _flowedV(0.0f), _flowedTime(0.0f)
		{

		}

// 		EntityYZMaterialLayer(const EntityYZMaterialLayer& c)
// 		{
// 
// 		}
		~EntityYZMaterialLayer()
		{
			if (_texture)
			{
				_texture->release();
			}
		}
	public:
		bool		_twoSides;
		bool		_flowU;
		bool		_flowV;
		Real		_flowSpeedU;
		Real		_flowSpeedV;
		u16			_flowTime;
		s16			_cols;
		s16			_rows;
		s16			_changeStyle;
		s16			_changeInterval;
		bool		_notCastShadow;
		bool		_unShaded;
		bool		_noDepthSet;
		bool		_noDepthTest;
		bool		_additiveLayer;
		std::string _tex;
		ITexture*   _texture;
		eBlendMode	_bm;
		Real		_flowedU;
		Real		_flowedV;
		Real		_flowedTime;
	};

	class EntityYZMaterial
	{
	public:
		/// Model Animations
		struct sAnimation
		{
			sAnimation()
			{
				alpha.setStaticData(1.0f);
				color.setStaticData(Color3(1.0f,1.0f,1.0f));
				flowSpeedU.setStaticData(1.0f);
				flowSpeedV.setStaticData(1.0f);
				changeStyle.setStaticData(0);
			}

			sKeyFrames<float>	alpha;
			sKeyFrames<Color3>	color;
			//版本号26加入
			sKeyFrames<float>	flowSpeedU,flowSpeedV,rotateSpeed;
			sKeyFrames<short>	changeStyle;
		};

	public:
		EntityYZMaterialLayer& getLayer(u32 index);	
		const std::string& getName() const;
		void setName(const std::string& name);
		void addLayer(const EntityYZMaterialLayer& layer);
	public:
		std::vector<EntityYZMaterialLayer>	_layers;	/// 材质层列表
		std::string							_name;
		sAnimation							_ani;
		Mat4								_texMatix;
	};
}
//# pragma pack(pop)
#endif // __EntityYZMaterial_h__
 
