#pragma once
#include <AL\alc.h>
#include <AL\al.h>
#include<iostream>
class testingAudio
{
public:
	testingAudio();
	void play();
private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};

