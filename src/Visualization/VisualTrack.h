#pragma once
#include "MidiFile/Midifile.h"
#include "Patterns/IPattern.h"

struct TrackInfo
{
	TrackInfo(unsigned int trackId, Patterns patternName, float timeDelay = 0) : trackId(trackId), patternName(patternName), timeDelay(timeDelay) {}
	//track id in the midi file
	unsigned int trackId = 0;

	Patterns patternName = LINES_PATTERN;
	//time delay relative to the Visualization
	float timeDelay = 0;
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

