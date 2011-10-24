#include "EntityYZMaterial.h"

namespace Euclid
{
	void EntityYZMaterial::setName( const std::string& name )
	{
		_name = name;
	}

	void EntityYZMaterial::addLayer( const EntityYZMaterialLayer& layer )
	{
		_layers.push_back(layer);
	}

	const std::string& EntityYZMaterial::getName() const
	{
		return _name;
	}

	EntityYZMaterialLayer& EntityYZMaterial::getLayer( u32 index )
	{
		return _layers[index];
	}
}
