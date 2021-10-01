#pragma once
#include <AL/al.h>
#include "Audio/SoundBuffer.h"

//You should use "audio" class, it manages memory.

//TODO: constractor
struct SourceInfo
{
	SourceInfo(float gain = 1.f) : p_Gain(gain)
	{};
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;
};

class SoundSource
{
public:
	//
	SoundSource(SourceInfo info);
	~SoundSource();

	void setBuffer(SoundBuffer* buffer_to_play);
	
	//By default set to lastBuffer; Use setBuffer()
	void Play(SoundBuffer* buffer_to_play = nullptr);
	void Pause();
	//check if the source is paused;
	bool isPaused();
protected:
	ALuint source;
	SourceInfo info;
private:
	SoundBuffer* lastBuffer = nullptr;
};

