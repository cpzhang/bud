//==========================================================================
/**
* @file	  : Game.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   18:27
* purpose : 
*/
//==========================================================================

#ifndef __Game_h__
#define __Game_h__

//
#include "Common.h"

//
namespace Buddha
{
	enum eProcess
	{
		eProcess_Begin,
		eProcess_CreateModules,
		eProcess_End,
	};
	class Game;
	class _BUHHDA_EXPORT_ IInputMessageHandler
	{
	public:
		IInputMessageHandler()
			:_game(NULL)
		{
		}

		virtual ~IInputMessageHandler()	
		{
		}

	public:
		//处理（分发）感兴趣的消息(回调机制)，WM_DESTROY 消息被系统内置，外界没法获得
		//如果处理了消息，不想让windows调用默认处理，请返回true，否则返回false
		virtual bool onInputMessage(void* hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
		{
			return false;
		}

	public:
		Game*	_game;
	};

	class _BUHHDA_EXPORT_ Game
	{
	public:
		Game();
		~Game();
	public:
		//
		virtual bool forePlay();
		
		//
		virtual bool play();
		
		//
		virtual bool postPlay();

		//
		virtual bool update(u32 current, u32 delta);

		//
		virtual bool foreRender();

		//
		virtual bool rendering();

		//
		virtual bool postRender();

		//
		virtual void onDragAndDrop();


		virtual void onF1();

		virtual bool destroy();

	public:
		//
		bool doing();

		//
		bool render();

		//
		bool createWindow();

		//
		bool adjustClientArea();

		//
		void setWindowTitle(const tstring& title);

		//
		bool isActive();

		//
		IInputMessageHandler* getInputMessageHandler();

		//
		void setInputMessageHandler(IInputMessageHandler* handler);

		//
		eProcess getProcess();
		void setProcess(eProcess e);
	public:
		// The window procedure, hands events the window receives
		static LRESULT CALLBACK _wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		//
		static std::string	_dragDropFile;

		//
		tstring		_windowName;

		//
		tstring		_windowTitle;

		//
		unsigned int	_clientWidth;

		//
		unsigned int	_clientHeight;

		//
		unsigned int	_windowX;

		//
		unsigned int	_windowY;

		//
		HWND			_hwnd;

		//
		float			_fps;
		//
		IInputMessageHandler* _inputMessageHandler;				/// 消息处理器

		//
		eProcess		_process;
	};
}

#endif // __Game_h__
