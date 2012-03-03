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
	WaitingForYou* wfy = (WaitingForYou*)_game;
	switch(message)
	{
		case WM_SIZE:
			{
				//onSize(LOWORD(lParam), HIWORD(lParam),wParam);
				if (wParam != SIZE_MINIMIZED)
				{
					Euclid::sViewPort& vp = wfy->_vp;
					vp.X = 0;
					vp.Y = 0;
					vp.Width = LOWORD(lParam);
					vp.Height = HIWORD(lParam);
					vp.MinZ = 0;
					vp.MaxZ = 1;
					if (1)
					{
						// 这里重置下设备，改变backbuffer大小，否则，可能会有严重的锯齿
						wfy->invalidateDevice();
						wfy->_modules->getRenderEngine()->getRenderSystem()->setViewport(&vp);
						wfy->_camera->setDirty();
						wfy->restoreDevice();
					}
				}
			}
			break;
		case WM_KEYDOWN:
			{
				if (0)
				{
					if (wfy->getCameraController())
						wfy->getCameraController()->onKeyDown(wParam);
				} 
				else
				{
					wfy->onKeyDown(wParam);
				}
				
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
						wfy->_fx_skybox->setBool("g_toggleFog", b);
						wfy->_fx->setBool("g_toggleFog", b);
					}break;
				case 'L':
					{
						static bool b = true;
						b = !b;
						wfy->_fx->setBool("g_toggleLightmapping", b);
					}break;
				case 'O':
					{
						//wfy->toggleLOD();
						wfy->fillIB();
					}break;
				case 'S':
					{
						wfy->send();
					}break;
				case 'R':
					{
						wfy->recieve();
					}break;
				case '5':
				case VK_NUMPAD5:
					{
						Buddha::WindowHelper::setAccelerationLevel(5);
					}break;
				case '0':
				case VK_NUMPAD0:
					{
						Buddha::WindowHelper::setAccelerationLevel(0);
					}break;
				case '1':
				case VK_NUMPAD1:
					{
						Buddha::WindowHelper::setAccelerationLevel(1);
					}break;
				case '2':
				case VK_NUMPAD2:
					{
						Buddha::WindowHelper::setAccelerationLevel(2);
					}break;
				case '3':
				case VK_NUMPAD3:
					{
						Buddha::WindowHelper::setAccelerationLevel(3);
					}break;
				case '4':
				case VK_NUMPAD4:
					{
						Buddha::WindowHelper::setAccelerationLevel(4);
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

				if(0)
				{
					WaitingForYou* wfy = (WaitingForYou*)_game;
					if (wfy->getCameraController())
					wfy->getCameraController()->onMouseMove(x,y);
				}
				else
				{
					static float sLastX = x;
					static float sLastY = y;
					float angleX = (float)(sLastX - x) * 0.002f;
					float angleY = (float)(y - sLastY) * 0.002f;
					sLastX = x;
					sLastY = y;
					
					//wfy->_camera->update();
					if (wfy && wfy->_camera)
					{
						wfy->_camera->pitch(angleY);
						wfy->_camera->yaw(angleX);
					}
					
					//wfy->_camera->rotate(Vec3::UNIT_X, angleY);
					//wfy->_camera->rotate(Vec3::UNIT_Y, angleX);
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
