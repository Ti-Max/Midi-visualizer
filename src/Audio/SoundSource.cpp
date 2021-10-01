#include "SoundSource.h"


SoundSource::SoundSource(SourceInfo info)
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, info.p_Pitch);
	alSourcef(source, AL_GAIN, info.p_Gain);
	alSource3f(source, AL_POSITION, info.p_Position[0], info.p_Position[1], info.p_Position[2]);
	alSource3f(source, AL_VELOCITY, info.p_Velocity[0], info.p_Velocity[1], info.p_Velocity[2]);
	alSourcei(source, AL_LOOPING, info.p_LoopSound);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &source);
}

void SoundSource::setBuffer(SoundBuffer* buffer_to_play)
{
	lastBuffer = buffer_to_play;
}

void SoundSource::Play(SoundBuffer* buffer_to_play)
{
	if (buffer_to_play != lastBuffer && buffer_to_play != nullptr)
	{
		lastBuffer = buffer_to_play;
		alSourcei(source, AL_BUFFER, lastBuffer->getBuffer());
	}
	if (lastBuffer == nullptr && buffer_to_play == nullptr)
	{
		std::cout << "Default buffer was not set! Use setBuffer(SoundBuffer) before calling Play() or send buffer as a prametr in Play(SoundBuffer) \n";
		return;
	}
	alSourcePlay(source);
}

void SoundSource::Pause()
{
	alSourcePause(source);
}

bool SoundSource::isPaused()
{
	ALint state = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PAUSED;
}
