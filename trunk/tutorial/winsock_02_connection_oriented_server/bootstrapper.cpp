//==========================================================================
/**
* @file	  : bootstrapper.cpp
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   18:24
* purpose : 
*/
//==========================================================================

#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include "gameImp.h"

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//
	GameImp* game = new GameImp;

	//
	if (!game->doing())
	{
	}

	//
	delete game;
	game = NULL;

	return 0;
}

#endif // __bootstrapper_cpp__
