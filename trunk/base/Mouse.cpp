#include "Mouse.h"

namespace Buddha
{


	Mouse::Mouse()
	{
		_point.x = 0;
		_point.y = 0;
		_pointLeftButtonDown.x = 0;
		_pointLeftButtonDown.y = 0;
		_pointRightButtonDown.x = 0;
		_pointRightButtonDown.y = 0;
	}

	Mouse::~Mouse()
	{

	}

	bool Mouse::isLeftButtonDown()
	{
		return static_cast<bool>((::GetAsyncKeyState(VK_LBUTTON) & 0x8000f));
	}

	bool Mouse::isRightButtonDown()
	{
		return static_cast<bool>((::GetAsyncKeyState(VK_RBUTTON) & 0x8000f));
	}

	POINT Mouse::getAbsPostion()
	{
		//
		GetCursorPos(&_point);

		//
		return _point;
	}

	POINT Mouse::getRelPosition(HWND hwnd)
	{
		//
		RECT rectWin;
		GetWindowRect(hwnd, &rectWin);

		//
		RECT rectClient;
		GetClientRect(hwnd, &rectClient);

		//
		int widthWin = rectWin.right - rectWin.left;
		int heightWin = rectWin.bottom - rectWin.top;

		//
		int widthClient = rectClient.right - rectClient.left;
		int heightClient = rectClient.bottom - rectClient.top;
		
		//
		int thin = static_cast<int>((widthWin - widthClient) * 0.5f);
		int thick = (heightWin - heightClient) - thin;

		//
		GetCursorPos(&_point);

		//
		_point.x -= (rectWin.left + thin);
		_point.y -= (rectWin.top + thick);

		//
		return _point;
	}

	POINT Mouse::getMoveLeftButtonDown()
	{
		//
		GetCursorPos(&_point);
		
		//
		_point.x -= _pointLeftButtonDown.x;
		_point.y -= _pointLeftButtonDown.y;

		//
		return _point;
	}


	POINT Mouse::getMoveRightButtonDown()
	{
		//
		GetCursorPos(&_point);

		//
		_point.x -= _pointRightButtonDown.x;
		_point.y -= _pointRightButtonDown.y;

		//
		return _point;
	}

	bool Mouse::update()
	{
		//
		if (isLeftButtonDown())
		{
			GetCursorPos(&_pointLeftButtonDown);
		}

		//
		if (isRightButtonDown())
		{
			GetCursorPos(&_pointRightButtonDown);
		}

		//
		return true;
	}

	bool Mouse::isKeyDown( const char key )
	{
		return static_cast<bool>(::GetAsyncKeyState(key) & 0x8000f);
	}
}
