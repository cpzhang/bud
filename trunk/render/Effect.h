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
	class _EuclidExport_ Effect
	{
	public:
		Effect()
		{
			_clear();
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
		virtual void			setTexture(const tstring& name, ITexture *pTexture);
		virtual void			setMatrix(const tstring& name, const Mat4 *pMatrix);
		virtual void			setMatrix(const tstring& name, const Mat4& pMatrix);
		virtual void			setInt(const tstring& name, int value);
		virtual void			setBool(const tstring& name, bool value);
		virtual bool			getBool(const tstring& name);
		virtual void			setFloat(const tstring& name, float value);
		virtual void			setValue(const tstring& name, void* data, u32 bytes);
		virtual void			setMatrixArray(const tstring& name, Mat4 *pMtxArray, u32 count);
		virtual void			setTechnique(const tstring&  name);
		virtual void			setFloatArray(const tstring&  name, const float *pFloatArray, u32 count);
		//
		virtual bool			setVector(const tstring&  name, const Vec4* pVector);
		virtual bool			setVector(const tstring&  name, Vec4* pVector);
		virtual bool			setVectorArray(const tstring&  name, const Vec4* pVector, u32 Count);
		virtual bool			setVectorArray(const tstring&  name, Vec4* pVector, u32 Count);

		virtual void			commitChanges();
	public:
		bool loadFromFile(const tstring& filename);
		bool loadFromMemory(const u8* data, u32 len);
		void onInvalidateDevice();
		void onRestoreDevice();
		void			addReference();
		typedef std::map<tstring, ITexture*> NameTextureMap;
		//
		void getMatrix(const tstring& name, const Mat4 *pMatrix);
	public:
		void _clear();
	private:
		ID3DXEffect*	_effect;

		NameTextureMap  _nameTextures;
		tstring		_effectfFile;
		s32				_references;
	};
}

#endif // __Effect_h__