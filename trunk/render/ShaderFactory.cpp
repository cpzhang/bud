#include "ShaderFactory.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace Euclid
{
	ShaderFactory::ShaderFactory()
	{

	}

	ShaderFactory::~ShaderFactory()
	{

	}

	IShader* ShaderFactory::createVertexShader( const std::string& name, const std::string& fileName )
	{
		std::string dataFile;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(dataFile);
		dataFile += fileName;

		VertexShader* s = new VertexShader(name, dataFile);

		return s;
	}

	IShader* ShaderFactory::createPixelShader( const std::string& name, const std::string& fileName )
	{
		std::string dataFile;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(dataFile);
		dataFile += fileName;

		PixelShader* s = new PixelShader(name, dataFile);

		return s;
	}
}