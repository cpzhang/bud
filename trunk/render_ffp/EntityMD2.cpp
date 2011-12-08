#include "stdafx.h"
#include "EntityMD2.h"
#include "MD2Loader.h"
#include "MD3Loader.h"
#include "Node.h"
#include "D3D9Mapping.h"
#include "IRenderSystem.h"
#include "BSPLoader.h"

namespace Euclid
{
		BSPLoader* bsp = 0;

	EntityMD2::EntityMD2()
		:	_md2(NULL), _currentIndex(0), _md3(NULL)
	{
		_type = "EntityMD2";
	}

	EntityMD2::~EntityMD2()
	{
		destroy();
	}

	bool EntityMD2::create()
	{
		//_md2 = new MD2Loader;

		//_md3 = new MD3Loader;
		bsp = new BSPLoader;

		return true;
	}

	void EntityMD2::destroy()
	{
		if (_md2)
		{
			_md2->destroy();
			delete _md2;
			_md2 = NULL;
		}
	}

	void EntityMD2::preRender()
	{
		if (_md2)
		{
			_md2->preRender();
		}
		if (_md3)
		{
			_md3->preRender();
		}

		if (bsp)
		{
			bsp->preRender();
		}

		if (_currentNode)
		{
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);
		}
	}

	void EntityMD2::postRender()
	{
		if (_md2)
		{
			_md2->postRender();
		}
		if (_md3)
		{
			_md3->postRender();
		}
		if (bsp)
		{
			bsp->postRender();
		}
	}

	void EntityMD2::render()
	{
		update();

		if (_md2)
		{
			_md2->renderFrame(_currentIndex);
		}

		if (_md3)
		{
			_md3->renderFrame(_currentIndex);
		}
		if (bsp)
		{
			bsp->render();
		}
	}

	bool EntityMD2::load( const std::string& fileName )
	{
		bool r = false;

		if (_md2)
		{
			r = _md2->load(fileName);
		}

		if (_md3)
		{
			r = _md3->load(fileName);
		}

		if (bsp)
		{
			r = bsp->load("mpteam3.bsp");
		}

		return r;
	}

	void EntityMD2::setFrameIndex( int index )
	{
		_currentIndex = index;
	}

	void EntityMD2::update()
	{
		static bool down = Buddha::Mouse::getInstancePtr()->isRightButtonDown();
		bool now = Buddha::Mouse::getInstancePtr()->isRightButtonDown();
		if (!down && now)
		{
			_currentIndex++;
		}
		
		down = now;
	}
}
