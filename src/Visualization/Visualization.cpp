#include "Visualization.h"
#include <ctime>
#include<iostream>

//DEBUG
template<typename T>
void print(T data) {
	std::cout << data << "\n";
}

void Visualization::addMidiTracks(const std::string& fileName, const std::vector<TrackInfo>& tracksInfo)
{
	smf::MidiFile* midifile = new smf::MidiFile(fileName);
	midifiles.push_back(midifile);
	if (!midifile->TIMO_isOpen)
	{
		return;
	}
	if (!midifile->status())
	{
		std::cout << "There are some problems with file: \"" << fileName << "\"\t but we still gonna try to add tracks (๑‵●‿●‵๑)\n` ";
	}

	midifile->doTimeAnalysis();
	midifile->linkNotePairs();

	std::cout << "File " << fileName << "\t has " << midifile->getTrackCount() << " tracks\n";

	for (int i = 0; i < tracksInfo.size(); i++)
	{
		if (tracksInfo[i].trackId <= midifile->getTrackCount())
		{
			if (midifile->getEventCount(tracksInfo[i].trackId) != 0)
			{
				VisualTrack* track = new VisualTrack(&(*midifile)[tracksInfo[i].trackId], tracksInfo[i]);
				this->tracks.push_back(track);
			}
		}
	}
}

void Visualization::Start()
{
	startTime = clock();
}

Visualization::~Visualization()
{
	for (auto file : midifiles)
		delete file;
	midifiles.clear();

	for (auto t : tracks)
		delete t;
	tracks.clear();
}
int mark;
void Visualization::Draw()
{
	if (startTime == 0)
	{
		std::cout << "Call function Start() before calling Draw!!\n";
		return;
	}

	for (auto track : tracks)
	{
		track->Update(TimeMark());
	}
}
void Visualization::Pause()
{
	pauseTime = clock();
	isPaused = true;
}

void Visualization::Resume()
{
	technikalDelay -= clock() - pauseTime;
	isPaused = false;
}

//Returns time in Clocks from beginig of visualization
long Visualization::TimeMark()
{
	return ((isPaused)?pauseTime : clock()) - startTime + timeDelay * CLOCKS_PER_SEC + technikalDelay;
}
