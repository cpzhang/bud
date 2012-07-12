#ifndef MusicEngine_h__
#define MusicEngine_h__
#include "Common.h"
#include "IMusicEngine.h"
#include "fmod.hpp"
class _MUSIC_EXPORT_ MusicEngine : public IMusicEngine
{
public:
	MusicEngine();
	~MusicEngine();
public:
	virtual bool				create();
	virtual bool				destroy();
	virtual void				update();
private:
	void clear();
private:
	FMOD::System* mSystem;
	bool		mHardware;
};

#endif // MusicEngine_h__
