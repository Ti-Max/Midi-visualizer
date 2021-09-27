#include "testingAudio.h"
#include <sndfile.h>

testingAudio::testingAudio()
{
	music.load("res/LudumDare1.wav");


	music.Play();

	///* Open the audio file and check that it's usable. */
	//sndfile = sf_open(filename, SFM_READ, &sfinfo);
	//if (!sndfile)
	//{
	//	fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
	//	return 0;
	//}
	//if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	//{
	//	fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
	//	sf_close(sndfile);
	//	return 0;
	//}

	///* Get the sound format, and figure out the OpenAL format */
	//format = AL_NONE;
	//if (sfinfo.channels == 1)
	//	format = AL_FORMAT_MONO16;
	//else if (sfinfo.channels == 2)
	//	format = AL_FORMAT_STEREO16;
	//else if (sfinfo.channels == 3)
	//{
	//	if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
	//		format = AL_FORMAT_BFORMAT2D_16;
	//}
	//else if (sfinfo.channels == 4)
	//{
	//	if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
	//		format = AL_FORMAT_BFORMAT3D_16;
	//}
	//if (!format)
	//{
	//	fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
	//	sf_close(sndfile);
	//	return 0;
	//}


}
