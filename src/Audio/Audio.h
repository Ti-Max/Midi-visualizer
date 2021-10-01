//TODO: Play(); default devices

#pragma once
#include "Audio/SoundContext.h"
#include "Audio/SoundBuffer.h"
#include "Audio/SoundSource.h"
#include <map>

class Audio
{
public:
	static Audio* get();
	//nullptr = default devise;
	static void init(const char* deviceName = nullptr);

	//Add buffer to the std::map; use 'name' when you call getBuffer()
	static SoundBuffer* addBuffer(SoundBuffer* buffer, const std::string& name);
	// 'name' is identificatior.
	static SoundSource* addSource(SoundSource* source, const std::string& name);

	static void Play(const std::string& source, const std::string& buffer = "default");
	static bool isPaused(const std::string& source);
	static void Pause(const std::string& source);
private:
	std::map<std::string, SoundSource*> soundSources;
	std::map<std::string, SoundBuffer*> soundBuffers;
	Audio();
	~Audio();
	SoundContext context;

};

