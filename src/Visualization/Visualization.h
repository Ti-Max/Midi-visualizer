#pragma once
#include "MidiFile/Midifile.h"
#include "VisualTrack.h"

#define ALL_TRACKS 0
#define BLOOM_TRACKS 1
#define NO_BLOOM_TRACKS 2

class Visualization
{
public:
	void addMidiTracks(const std::string& fileName, const std::vector<TrackInfo> &tracksInfo,  bool sortTrackBychannel);
	//call this before main loop
	void Start();
	~Visualization();

	//call every frame

	void Draw(unsigned int tracksType);

	void Pause();
	void Resume();

	//Time delay for visualization in seconds
	float timeDelay = 0;
private:

	std::vector<smf::MidiFile*> midifiles;
	std::vector<VisualTrack*> tracksWithBloom;
	std::vector<VisualTrack*> tracks;
	
	//Time in clock (0.001 seconds) when visualization started;
	long startTime = 0;
	long pauseTime = 0;
	bool isPaused = false;
	//Used to handle pauses and rewinds
	long technikalDelay = 0;
	long TimeMark();
};

