#pragma once
#include "Audio/SoundContext.h"
#include "Audio/SoundSource.h"
#include "Audio/SoundBuffer.h"
#include <map>
class Audio
{
public:
	static Audio* get();
	static void init();

	
	static SoundBuffer* addBuffer(std::string path, std::string name);


	std::map<std::string, SoundSource*> SoundSources;
	std::map<std::string, SoundBuffer*> SoundBuffers;
private:
	Audio();
	~Audio();
	SoundContext context;

};

