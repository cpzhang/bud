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
			Euclid::sViewPort vp;
			vp.X = 0;
			vp.Y = 0;
			vp.Width = LOWORD(lParam);
			vp.Height = HIWORD(lParam);
			vp.MinZ = 0;
			vp.MaxZ = 1;
			//wfy->invalidateDevice();
			wfy->_modules->getRenderEngine()->getRenderSystem()->setViewport(&vp);
			//wfy->_camera->setDirty();
			//wfy->restoreDevice();
		}
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
			// Destroy the window and Send the WM_DESTROY message
		case VK_ESCAPE:
			{
				::DestroyWindow((HWND)hWnd);
			}
			break;
		}
	}

	return false;
}
