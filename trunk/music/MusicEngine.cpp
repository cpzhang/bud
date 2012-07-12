#include "MusicEngine.h"

MusicEngine::MusicEngine()
{
	clear();
}

MusicEngine::~MusicEngine()
{

}

bool MusicEngine::create()
{
	FMOD_RESULT result = FMOD::System_Create(&mSystem);
	if (FMOD_OK != result)
	{
		return false;
	}
	FMOD_CAPS caps;
	int controlpaneloutputrate;
	FMOD_SPEAKERMODE controlpanelspeakermode;
	mSystem->getDriverCaps(0, &caps, &controlpaneloutputrate, &controlpanelspeakermode);
	if (caps & FMOD_CAPS_HARDWARE)
	{
		mHardware = true;
	}
	result = mSystem->init(100, FMOD_INIT_NORMAL, 0);
	if (FMOD_OK != result)
	{
		return false;
	}

	return true;
}

bool MusicEngine::destroy()
{
	if (mSystem)
	{
		mSystem->release();
	}
	clear();
	return true;
}

void MusicEngine::clear()
{
	mSystem = NULL;
	mHardware = false;
}

void MusicEngine::update()
{
	if (mSystem)
	{
		mSystem->update();
	}
}

extern "C" _MUSIC_EXPORT_ IMusicEngine* APIENTRY createMusicEngine()
{
	IMusicEngine* e = new MusicEngine;
	if (e && e->create())
	{
		return e;
	}
	return NULL;
}