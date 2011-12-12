/********************************************************************
**	file: 		Effect.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __Effect_h__
#define __Effect_h__

#include "Common.h"
namespace Euclid
{
	class Effect
	{
	public:
		Effect()
			:_effect(NULL)
		{
		}

		~Effect();
	public:
		virtual void			destroy();
		virtual bool			begin(u32* count);
		virtual void			end();
		virtual bool			beginPass(u32 index);
		virtual void			endPass();
		virtual void			onLostDevice();
		virtual void			onResetDevice();
		virtual void			setTexture(const std::string& name, ITexture *pTexture);
		virtual void			setMatrix(const std::string& name, const Mat4 *pMatrix);
		virtual void			setMatrix(const std::string& name, const Mat4& pMatrix);
		virtual void			setInt(const std::string& name, int value);
		virtual void			setBool(const std::string& name, bool value);
		virtual bool			getBool(const std::string& name);
		virtual void			setFloat(const std::string& name, float value);
		virtual void			setValue(const std::string& name, void* data, u32 bytes);
		virtual void			setMatrixArray(const std::string& name, Mat4 *pMtxArray, u32 count);
		virtual void			setTechnique(const std::string&  name);
		virtual void			setFloatArray(const std::string&  name, float *pFloatArray, u32 count);
		virtual void			commitChanges();
	public:
		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const u8* data, u32 len);

		typedef std::map<std::string, ITexture*> NameTextureMap;
	private:
		ID3DXEffect*	_effect;

		NameTextureMap  _nameTextures;
	};
}

#endif // __Effect_h__