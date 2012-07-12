#pragma once
#include "Common.h"
#include "RenderEngineCreationParameters.h"
namespace Euclid
{
	struct sVDTNull
	{
		virtual size_t size()
		{
			return 0;
		}
	};
	struct sVDTPosition : public sVDTNull
	{
		sVDTPosition()
		{
			mVDT = eVertexDeclarationType_Position;
		}
		virtual size_t size()
		{
			return sizeof(mData);
		}
		eVertexDeclarationType mVDT;
		sPosition mData;
	};
	struct sVDTPositionTexture : public sVDTNull
	{
		sVDTPositionTexture()
		{
			mVDT = eVertexDeclarationType_PositionTexture;
		}
		virtual size_t size()
		{
			return sizeof(mData);
		}
		eVertexDeclarationType mVDT;
		sPositionTexture mData;
	};
	struct sVDTPositionTTexture : public sVDTNull
	{
		sVDTPositionTTexture()
		{
			mVDT = eVertexDeclarationType_PositionTTexture;
		}
		virtual size_t size()
		{
			return sizeof(mData);
		}
		eVertexDeclarationType mVDT;
		sPositionTTexture mData;
	};
	struct sVDTPositionTextureBoneWeightColor : public sVDTNull
	{
		sVDTPositionTextureBoneWeightColor()
		{
			mVDT = eVertexDeclarationType_PositionTextureBoneWeightColor;
		}
		virtual size_t size()
		{
			return sizeof(mData);
		}
		eVertexDeclarationType mVDT;
		sPositionTextureBoneWeightColor mData;
	};
	struct sVDTPositionTextureBoneWeightColorNormal : public sVDTNull
	{
		sVDTPositionTextureBoneWeightColorNormal()
		{
			mVDT = eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
		}
		virtual size_t size()
		{
			return sizeof(mData);
		}
		eVertexDeclarationType mVDT;
		sPositionTextureBoneWeightColorNormal mData;
	};
	typedef std::vector<sVDTNull*> VDTLayoutMap;
	class VDTManager: public Buddha::SingletonEx<VDTManager>
	{
	public:
		VDTManager();
		~VDTManager();
		
	public:
		VDTLayoutMap mLayouts;
	};
}
