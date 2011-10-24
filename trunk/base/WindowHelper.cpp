#include "WindowHelper.h" 

//
namespace Buddha
{
	WindowHelper::WindowHelper()
	{

	}

	WindowHelper::~WindowHelper()
	{

	}

	int WindowHelper::getWindowHeight( HWND hwnd )
	{
		//
		RECT rectWin;
		GetWindowRect(hwnd, &rectWin);

		//
		return rectWin.bottom - rectWin.top;
	}

	int WindowHelper::getWindowWidth( HWND hwnd )
	{
		//
		RECT rectWin;
		GetWindowRect(hwnd, &rectWin);

		//
		return rectWin.right- rectWin.left;
	}

	int WindowHelper::getClientWidth( HWND hwnd )
	{
		//
		RECT rectClient;
		GetClientRect(hwnd, &rectClient);

		//
		return rectClient.right - rectClient.left;
	}

	int WindowHelper::getClientHeight( HWND hwnd )
	{
		//
		RECT rectClient;
		GetClientRect(hwnd, &rectClient);

		//
		return rectClient.bottom- rectClient.top;
	}

	int WindowHelper::getScreenWidth()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}

	int WindowHelper::getScreenHeight()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}

	float WindowHelper::getSceenWidthHeightRatio()
	{
		float w = static_cast<float>(getScreenWidth());
		float h = static_cast<float>(getScreenHeight());

		return w / h;
	}

}