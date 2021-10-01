#pragma once
#include <AL/alc.h>
#include <AL/al.h>
#include <iostream>
class SoundContext
{
public:
	SoundContext();
	//Creates a Context and makes it current 
	SoundContext(const char* deviceName);
	~SoundContext();
	void Init(const ALCchar* deviceName);
private:

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};

