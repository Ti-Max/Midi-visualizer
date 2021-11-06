#pragma once
#include "MidiFile/Midifile.h"
#include "Patterns/ListOfPatterns.h"

struct TrackInfo
{
	TrackInfo(unsigned int trackId, Patterns patternName, ISettings* patternSettings,  float timeDelay, bool bloom) : trackId(trackId), patternName(patternName), settings(patternSettings), timeDelay(timeDelay), bloom(bloom){}
	//track id in the midi file
	unsigned int trackId = 0;

	Patterns patternName = LINES_PATTERN;
	ISettings *settings = nullptr;
	//time delay relative to the Visualization
	float timeDelay = 0;
	bool bloom = false;
};

class VisualTrack
{
public:
	VisualTrack(smf::MidiEventList* track, const TrackInfo& info);
	~VisualTrack();
	void Start();
	void Update(long visualizationTime);
private:

	smf::MidiEventList* track;
	int nEvent = 0;

	//TrackInfo 
	//Time delay relative to the Visualization
	float timeDelay = 0;

	//----------------------------
	smf::MidiEvent* NewNote(long visualizationTime);

	//current notes
	std::vector<NoteInfo> currentNotes;

	IPattern* pattern;
};

