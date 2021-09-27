#pragma once
#include <AL/al.h>
#include "Audio/SoundBuffer.h"
class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(SoundBuffer* buffer_to_play);
protected:
	ALuint source;
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;
private:
	SoundBuffer* lastBuffer = nullptr;
};

