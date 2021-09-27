#pragma once
#include <AL/al.h>
#include <AL\alext.h>

#include <sndfile.h>
#include <iostream>
class SoundBuffer
{
public:
	SoundBuffer(const std::string& fileName);
	SoundBuffer();
	~SoundBuffer();
	ALuint getBuffer();
	bool load(const	std::string& fileName);
private:
	bool readFile(const char* path, ALenum &format, short** data, ALsizei &size, ALsizei &freq);
	ALuint buffer;
};

