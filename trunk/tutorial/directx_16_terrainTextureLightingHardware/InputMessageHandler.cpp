#include "InputMessageHandler.h"
#include "game.h"

InputMessageHandler::InputMessageHandler()
{

}

InputMessageHandler::~InputMessageHandler()
{

}

bool InputMessageHandler::onInputMessage( void* hWnd, unsigned int message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
		case WM_KEYDOWN:
			{
				WaitingForYou* wfy = (WaitingForYou*)_game;
				if (wfy->getCameraController())
					wfy->getCameraController()->onKeyDown(wParam);

				switch(wParam)
				{
					// Destroy the window and Send the WM_DESTROY message
				case VK_ESCAPE:
					{
						::DestroyWindow((HWND)hWnd);
					}
					break;
				case 'T':
					{
						wfy->toggleFillMode();
					}break;
				case 'G':
					{
						static bool b = true;
						b = !b;
						wfy->_fx->setBool("g_toggleTexture", b);
					}break;
				case 'L':
					{
						static bool b = true;
						b = !b;
						wfy->_fx->setBool("g_toggleLight", b);
					}break;
				}
			}
		case WM_LBUTTONDOWN:
			{
				SetCapture((HWND)hWnd);
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
						wfy->getCameraController()->onLeftButtonDown(x,y);
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				ReleaseCapture();
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onLeftButtonUp(x,y);
				}
			}
			break;
		//case WM_MOUSELEAVE:
		case WM_MOUSEMOVE:
			{
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onMouseMove(x,y);
				}
			}
			break;
		case WM_RBUTTONDOWN:
			{
				SetCapture((HWND)hWnd);
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onRightButtonDown(x,y);
				}
			}
			break;
		case WM_RBUTTONUP:
			{
				ReleaseCapture();
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onRightButtonUp(x,y);
				}
			}
			break;
		case WM_MBUTTONDOWN:
			{
				SetCapture((HWND)hWnd);	
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				WaitingForYou* wfy = (WaitingForYou*)_game;
				if (wfy->getCameraController())
				wfy->getCameraController()->onMButtonDown(x,y);
			}
			break;
		case WM_MBUTTONUP:
			{
				ReleaseCapture();
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				WaitingForYou* wfy = (WaitingForYou*)_game;
				if (wfy->getCameraController())
				wfy->getCameraController()->onMButtonUp(x,y);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				short delta = HIWORD(wParam);
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onMouseWheel(delta);
				}
			}
			break;
	}	

	return false;
}
