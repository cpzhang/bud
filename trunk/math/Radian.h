//==========================================================================
/**
* @file	  : Radian.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   18:08
* purpose : 
*/
//==========================================================================

#ifndef __Radian_h__
#define __Radian_h__

#include "Common.h"

//
namespace Euler
{
	class Radian
	{
	public:
		Real _radian;

		//
	public:
		Radian ( Real r=0 )
			: _radian(r) 
		{}
		//
	public:
		//
		inline	Radian& operator = ( const Real& f )
		{
			_radian = f; 
			return *this;
		}

		//
		inline Radian& operator = ( const Radian& r ) 
		{
			_radian = r._radian; 
			return *this; 
		}

		//
		inline Radian& operator + ( const Radian& r ) 
		{
			_radian += r._radian; 
			return *this; 
		}

		//
		inline Radian& operator += ( const Radian& r ) 
		{
			_radian += r._radian; 
			return *this; 
		}

		//
		inline Radian& operator -= ( const Radian& r ) 
		{
			_radian -= r._radian; 
			return *this; 
		}

		inline Radian& operator += ( const Real& f )
		{
			_radian += f;

			return *this;
		}

		//
		inline Radian operator * ( Real f ) const 
		{
			return Radian ( _radian * f );
		}

		//
		inline Radian operator / ( Real f ) const 
		{
			return Radian ( _radian / f );
		}
		//
		Radian operator * ( const Radian& f ) const
		{
			return Radian ( _radian * f._radian );
		}

		//
		Radian operator - () const
		{
			return Radian ( -_radian);
		}

		//
		friend Radian operator * ( Real a, const Radian& b )
		{
			return Radian ( a * b._radian );
		}
	};
}

#endif // __Radian_h__
 
