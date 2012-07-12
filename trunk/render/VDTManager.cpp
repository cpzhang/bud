#include "VDTManager.h"
namespace Euclid
{


	VDTManager::VDTManager()
	{
		mLayouts.resize(eVertexDeclarationType_Size);
		for (size_t i = 0; i != mLayouts.size(); ++i)
		{
			mLayouts[i] = NULL;
		}
		//mLayouts[eVertexDeclarationType_Null] = new sNull;
		mLayouts[eVertexDeclarationType_Position] = new sVDTPosition;
		mLayouts[eVertexDeclarationType_PositionTexture] = new sVDTPositionTexture;
		mLayouts[eVertexDeclarationType_PositionTTexture] = new sVDTPositionTTexture;
		mLayouts[eVertexDeclarationType_PositionTextureBoneWeightColor] = new sVDTPositionTextureBoneWeightColor;
		mLayouts[eVertexDeclarationType_PositionTextureBoneWeightColorNormal] = new sVDTPositionTextureBoneWeightColorNormal;
	}

	VDTManager::~VDTManager()
	{
		for (size_t i = 0; i != mLayouts.size(); ++i)
		{
			if (mLayouts[i])
			{
				delete mLayouts[i];
				mLayouts[i] = NULL;
			}
		}
	}

}