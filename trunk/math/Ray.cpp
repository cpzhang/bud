#include "Ray.h" 

namespace Zen
{
	Ray::Ray()
		:	_origin(Vector3::ZERO), _direction(Vector3::UNIT_Z)
	{

	}

	Ray::Ray( const Vector3& dir, const Vector3& origin )
		:	_origin(origin), _direction(dir)
	{

	}

	Vector3 Ray::getPoint( Real t ) const
	{
		return _origin + t * _direction;
	}
}