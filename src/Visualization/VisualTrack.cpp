#include "VisualTrack.h"

#include "Patterns/ListOfPatterns.h"
using namespace glm;

VisualTrack::VisualTrack(smf::MidiEventList* track, const TrackInfo& info)
{
	this->track = track;

	timeDelay = info.timeDelay;

	switch (info.patternName)
	{
	case LINES_PATTERN:
		pattern = new LinesPattern;
		break;
	default:
		pattern = nullptr;
		break;
	}
	if (pattern != nullptr)
		pattern->SetUp();
}

void VisualTrack::Start()
{

}

void VisualTrack::Update(long visualizationTime)
{
	visualizationTime += timeDelay * CLOCKS_PER_SEC;

	//Now we are checking if we got any new notes
	smf::MidiEvent* note;
	while ((note = NewNote(visualizationTime)) != nullptr)
	{
		if (note->isNoteOn())
		{
			//Checking if this note already pressed, then erase it 
			//MAY be there is a better way
			for (auto it = currentNotes.begin(); it != currentNotes.end(); ++it)
			{
				if (note->getKeyNumber() == (*it).key)
				{
					currentNotes.erase(it);
					break;
				}
			}
			currentNotes.push_back(NoteInfo(note->getKeyNumber(), note->getVelocity(), note->seconds, note->getDurationInSeconds()));
		}
		else
		{
			//Erase note from array if it is NoteOff event
			for (auto it = currentNotes.begin(); it != currentNotes.end(); it++)
			{
				if ((*it).key == note->getKeyNumber())
				{
					currentNotes.erase(it);
					break;
				}
			}
		}
	}

	//------------------------------------------------------------------
	//Render notes in the currentNotes array
	for (auto it = currentNotes.begin(); it != currentNotes.end(); it++)
	{
		pattern->Draw(*it, visualizationTime);
	}
}

smf::MidiEvent* VisualTrack::NewNote(long visualizationTime)
{
	//goes through the array of midi events stating from last event
	for (nEvent; nEvent < track->getEventCount(); nEvent++)
	{
		smf::MidiEvent* currentEvent = &(track->getEvent(nEvent));
		//is it "note on" event?
		if (currentEvent->isNoteOn() || currentEvent->isNoteOff())
		{
			//check if the time for this note has come
			if (currentEvent->seconds * CLOCKS_PER_SEC < visualizationTime)
			{
				//check if two notes pressed at the same time (optional)
				//if (track->getEvent(nEvent).tick != lastTick || firstNote)
				//lastTick = track->getEvent(nEvent).tick;
				//const char* state = (track->getEvent(nEvent).isNoteOn())? "On" : "off";
				//std::cout << "Key " << currentEvent->getKeyNumber()<<"\t"<< state << "\t sec : " << currentEvent->seconds << std::endl;
				//if (firstNote)
				//	firstNote = false;
				return &(track->getEvent(nEvent++));
			}
			else
			{
				//we should wait, maybe next frame...
				break;
			}
		}

	}
	//std::cout << "Track is finished\n";
	return nullptr;
}

VisualTrack::~VisualTrack()
{
	delete pattern;
}
