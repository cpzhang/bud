#include "InputMessageHandler.h"

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
