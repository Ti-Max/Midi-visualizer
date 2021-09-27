#pragma once
#include <AL\alc.h>
#include <AL\al.h>
#include<iostream>
#include "Audio/SoundContext.h"
//#include "Audio/SoundBuffer.h"
//#include "Audio/SoundSource.h"
#include "Audio/MusicSource.h"

class testingAudio
{
public:
	testingAudio();
private:
	SoundContext soundContext;
	MusicSource music;
	//SoundBuffer buffer;
	//SoundSource source;
};

