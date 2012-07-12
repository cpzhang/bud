#ifndef IMusicEngine_h__
#define IMusicEngine_h__
#include "Common.h"
class _MUSIC_EXPORT_ IMusicEngine
{
public:
	IMusicEngine()
	{

	}

	virtual ~IMusicEngine()
	{

	}
public:
	virtual bool				create() = 0;
	virtual bool				destroy() = 0;
	virtual void				update() = 0;
	virtual void				play() = 0;
};

// Factory function that creates instances of the IRenderEngine object.
extern "C" _MUSIC_EXPORT_ IMusicEngine* APIENTRY createMusicEngine();

#endif // IMusicEngine_h__
