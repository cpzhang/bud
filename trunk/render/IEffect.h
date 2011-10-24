/********************************************************************
**	file: 		IEffect.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-2-24
**	purpose:	
*********************************************************************/
#ifndef __IEffect_h__
#define __IEffect_h__

#include "Common.h"
//
namespace Euclid
{
	class IEffect
	{
	public:
		virtual void			release() = 0;
		virtual void			release(RenderableObject* o) = 0;
		virtual bool			begin(u32* count) = 0;
		virtual void			end() = 0;
		virtual bool			beginPass(u32 index) = 0;
		virtual void			endPass() = 0;
		virtual void			onLostDevice() = 0;
		virtual void			onResetDevice() = 0;
		virtual void			setTexture(const std::string& name,ITexture *pTexture) = 0;
		virtual void			setMatrix(const std::string& name, Mat4 *pMatrix) = 0;
		virtual void			setInt(const std::string& name, int value) = 0;
		virtual void			setBool(const std::string& name, bool value) = 0;
		virtual bool			getBool(const std::string& name) = 0;
		virtual void			setFloat(const std::string& name, float value) = 0;
		virtual void			setValue(const std::string& name, void* data, u32 bytes) = 0;
		virtual void			setMatrixArray(const std::string& name, Mat4 *pMtxArray, u32 count) = 0;
		virtual void			setTechnique(const std::string&  name) = 0;
		virtual void			setFloatArray(const std::string&  name, float *pFloatArray, u32 count) = 0;
		virtual void			commitChanges() = 0;
		virtual void			onSetEffect(RenderableObject* o) = 0;
	};
}


#endif // __IEffect_h__ 
