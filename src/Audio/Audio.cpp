#include "Audio.h"

static Audio* _instance = nullptr;

Audio* Audio::get()
{
	init();
	return _instance;
}

void Audio::init()
{
	if (_instance == nullptr)
		_instance = new Audio();
}

Audio::Audio()
{
}

Audio::~Audio()
{
	for (auto buffer : SoundBuffers) {
		delete buffer.second;
	}
	for (auto source : SoundSources) {
		delete source.second;
	}
}
