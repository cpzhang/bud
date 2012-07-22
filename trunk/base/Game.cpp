#include "Game.h" 
#include "FileSystem.h"
#include "Logger.h"
#include "WindowHelper.h"
//
namespace Zen
{
	Game*	gGameClient = NULL;

	Game::Game()
		: _windowName(TEXT("Game_Window")), _windowTitle(TEXT("Game")), _clientHeight(600), _clientWidth(480),
		_windowX(0), _windowY(0), _hwnd(0), _fps(0.0f), _inputMessageHandler(NULL),
		_process(eProcess_Begin)
	{
		gGameClient = this;
		_clientWidth = _clientHeight * static_cast<unsigned int>(WindowHelper::getSceenWidthHeightRatio());
	}

	Game::~Game()
	{
		if (::IsWindow((HWND)_hwnd))
		{
			::DestroyWindow((HWND)_hwnd);
		}
		gGameClient = NULL;
		if (Logger::getInstancePtr())
		{
			delete Logger::getInstancePtr();
		}
	}

	bool Game::doing()
	{
		if (!forePlay())
		{
			return false;
		}
		// Message Structure
		MSG msg;
		::ZeroMemory(&msg, sizeof(msg));
		// Loop until getting a WM_QUIT message
		while(true)
		{
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				if (!play())
				{
					break;
				}
			}
		}
		// clean up
		if (!postPlay())
		{
			return false;
		}

		return false;
	}

	bool Game::forePlay()
	{
		//
		new Logger;

		if (!createWindow())
		{
			return false;
		}

		//
		if (!adjustClientArea())
		{
			return false;
		}

		return true;
	}

	bool Game::play()
	{
		static float last_time = (float)timeGetTime();
		float current_time = (float)timeGetTime();
		static int count_frames = 0;
		++count_frames;
		float delta = current_time - last_time;
		// calculate FPS per Second
		if(delta >= 1000.0f)
		{
			_fps = (float)count_frames * 1000.0f  / delta;
			last_time = current_time;
			count_frames = 0;
		}

		static float last_frame_stamp = current_time;
		//
		if (!update((u32)current_time, (u32)(current_time - last_frame_stamp)))
		{
			return false;
		}
		last_frame_stamp = current_time;

		//
		if (!render())
		{
			return false;
		}

		//
		return true;
	}

	bool Game::postPlay()
	{
		if (!destroy())
		{
			return false;
		}

		return true;
	}

	bool Game::createWindow()
	{
		// Fill out the WNDCLASS structure
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = _wndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = ::GetModuleHandle(NULL);
		wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName = 0;
		wc.lpszClassName = _windowName.c_str();

		// Register the WNDCLASS description 
		if (!::RegisterClass(&wc))
		{
			return false;
		}

		// Create the window
		_hwnd = ::CreateWindow(_windowName.c_str(), _windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			_windowX, _windowY, _clientWidth, _clientHeight,
			GetDesktopWindow(), 0, ::GetModuleHandle(0), 0);
		if (_hwnd ==0)
		{
			return false;
		}

		// Show the window
		::ShowWindow(_hwnd, SW_NORMAL);
		// Update the window
		::UpdateWindow(_hwnd);

		//
		return true;
	}

	LRESULT CALLBACK Game::_wndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch(message)
		{
			// Send the WM_QUIT message, terminating the message loop
		case WM_DESTROY:
			{
				::PostQuitMessage(0);
			}
			break;
// 
// 		case WM_PAINT:
// 			if (gGameClient && gGameClient->getProcess() >= eProcess_CreateModules)
// 			{
// 				//gGameClient->render();
// 			}break;
		default:
			{
				if (gGameClient && gGameClient->getInputMessageHandler() && gGameClient->getInputMessageHandler()->onInputMessage(hWnd, message, wParam, lParam))
				{
					return false;
				}
			}
			break;
		}
		// Forward any other messages we didn't handle above to the default window procedure
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	bool Game::adjustClientArea()
	{
		//
		RECT rect = {0, 0, _clientWidth, _clientHeight};
		if (!AdjustWindowRect(&rect, 
			GetWindowLong(_hwnd, GWL_STYLE), GetMenu(_hwnd) != NULL))
		{
			return false;
		}

		//
		unsigned int width = GetSystemMetrics(SM_CXFULLSCREEN);
		unsigned int height = GetSystemMetrics(SM_CYFULLSCREEN);

		//
		RECT rectWin;
		GetWindowRect(_hwnd, &rectWin);

		double x = (width - (rectWin.right - rectWin.left)) * 0.5;
		double y = (height - (rectWin.bottom - rectWin.top)) * 0.5;
		//
		if (!MoveWindow(_hwnd, (unsigned int)x, (unsigned int)y,
			rect.right - rect.left,
			rect.bottom - rect.top,
			true))
		{
			return false;
		}
		
		//
		return true;
	}

	bool Game::render()
	{
		if (!isActive())
		{
			return true;
		}

		//
		if (!foreRender())
		{
			return false;
		}

		//
		if (!rendering())
		{
			return false;
		}

		//
		if (!postRender())
		{
			return false;
		}

		//
		return true;
	}

	bool Game::update(u32 current, u32 delta)
	{
		return true;
	}

	bool Game::foreRender()
	{
		return true;
	}

	bool Game::rendering()
	{
		return true;
	}

	bool Game::postRender()
	{
		return true;
	}

	void Game::setWindowTitle( const tstring& title )
	{
		_windowTitle = title;
	}

	void Game::onDragAndDrop()
	{

	}

	void Game::onF1()
	{

	}

	bool Game::isActive()
	{
		return ::GetActiveWindow() == _hwnd;
	}

	bool Game::destroy()
	{
		return true;
	}

	IInputMessageHandler* Game::getInputMessageHandler()
	{
		return _inputMessageHandler;
	}

	void Game::setInputMessageHandler( IInputMessageHandler* handler )
	{
		_inputMessageHandler = handler;
		_inputMessageHandler->_game = this;
	}

	Zen::eProcess Game::getProcess()
	{
		return _process;
	}

	void Game::setProcess( eProcess e )
	{
		_process = e;
	}

	std::string Game::_dragDropFile("");

}