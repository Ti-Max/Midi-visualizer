//DOES NOT WORK
//НЕ РАБОТАЕТ


#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AL\alext.h>
#include <sndfile.h>
#include <iostream>

class MusicSource
{
public:
	MusicSource(const std::string& filename);
	MusicSource();
	void Play();
	void UpdateStream();
	void load(const std::string& filename);
	~MusicSource();
private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;
};

