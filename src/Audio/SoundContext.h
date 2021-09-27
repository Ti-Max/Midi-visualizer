#pragma once
#include <AL/alc.h>
#include <AL/al.h>
#include <iostream>
class SoundContext
{
public:
	//Creates a Context and makes it current 
	SoundContext(const char* deviceName = nullptr);
	~SoundContext();
private:
	void Init(const ALCchar* deviceName);

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};

