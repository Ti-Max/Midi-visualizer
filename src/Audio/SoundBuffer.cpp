#include "SoundBuffer.h"

SoundBuffer::SoundBuffer()
{

}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &buffer);
}

ALuint SoundBuffer::getBuffer()
{
	return buffer;
}

SoundBuffer::SoundBuffer(const std::string& fileName)
{
	load(fileName);
}

bool SoundBuffer::load(const std::string& fileName)
{
	const char* path = fileName.c_str();

	ALenum err, format;
	short* data = nullptr;
	ALsizei size, freq;
	
	readFile(path, format, &data, size, freq);

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, size, freq);
	free(data);


	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}
}

bool SoundBuffer::readFile(const char* path, ALenum &format, short** data, ALsizei &size, ALsizei &freq)
{
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	sf_count_t num_frames;
	/* Open the audio file and check that it's usable. */
	sndfile = sf_open(path, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		std::cout << "Could not open audio in: " << path << sf_strerror(sndfile) << std::endl;
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s \n", path, sfinfo.frames);
		sf_close(sndfile);
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/* Decode the whole audio file to a buffer. */
	*data = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, *data, sfinfo.frames);
	if (num_frames < 1)
	{
		free(*data);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s \n", path, num_frames);
		return 0;
	}
	freq = sfinfo.samplerate;
	size = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
	sf_close(sndfile);
}
