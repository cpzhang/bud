#include "MPQLoader.h" 

namespace Euclid
{
	MPQLoader::MPQLoader()
	{

	}

	MPQLoader::~MPQLoader()
	{

	}

	bool MPQLoader::load( const std::string& name )
	{
		std::ifstream f(name.c_str(), std::ios_base::binary);

		//
		if (!f.good())
		{
			return false;
		}

		f.read((char*)&_header, sizeof(sHeader));

		f.close();

		return true;
	}
}