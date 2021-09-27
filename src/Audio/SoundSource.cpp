#include "SoundSource.h"


SoundSource::SoundSource()
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, p_Pitch);
	alSourcef(source, AL_GAIN, p_Gain);
	alSource3f(source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(source, AL_LOOPING, p_LoopSound);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &source);
}

void SoundSource::Play(SoundBuffer* buffer_to_play)
{
	if (buffer_to_play != lastBuffer)
	{
		lastBuffer = buffer_to_play;
		alSourcei(source, AL_BUFFER, lastBuffer->getBuffer());
	}

	alSourcePlay(source);
}
