//Seems like Audio goes faster. FIX IT
#include "Visualization.h"

#include <ctime>
#include<iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include<chrono>
using namespace glm;

void Visualization::Start(smf::MidiEventList* track)
{
	this->track = track;

	quadMesh.load();
	quad = quadMesh["quad"];

	shader.load("quadShader");
	shader.use();
	shader.setVec3("color", glm::vec3(0, 1, 0));

	std::srand(time(0));

	startTime = clock();

}
template<typename T>
void print(T data) {
	std::cout << data << "\n";
}

int scrollPos = 0;
int scrollScale = 0;
void Visualization::Draw(int scroll, bool isCtr)
{
	if (startTime == 0)
	{
		std::cout << "Call function Start() before calling Draw!!\n";
		return;
	}
	//Now we are checking if we got any new notes
		smf::MidiEvent* note;
		while ((note = NewNote()) != nullptr)
		{
			if (note->isNoteOn())
			{
				//Checking if this note already pressed, then erase it 
				//MAY there is a better way
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


		for (auto it = currentNotes.begin(); it != currentNotes.end(); it++)
		{
			int range[2] = { 50, 100};// 0 - 50
			//if (scroll)
			//{
			//	if (isCtr)
			//		scrollPos += scroll;
			//	else
			//		scrollScale += scroll;
			//}
			//if (scrollPos)
			//{
			//	range[0] += scrollPos * 2;
			//	range[1] += scrollPos * 2;
			//}
			//if(scrollScale)
			//{
			//	range[0] += scrollScale * 2;
			//	range[1] -= scrollScale * 2;
			//}
			//print(scroll);
			int noteCount = range[1] - (range[0]);
			int& key = (*it).key;//1 - 127

			float fade = 1 - ((float)now() / CLOCKS_PER_SEC - (*it).noteOnTime) /(*it).duration;
			//float fade = 1;
			float scaling;
			float opacity = 1;
			if (fade < 0.5) {
				scaling = 4 * (*it).velocity/100 * 0.25;
				opacity = fade * 2 * (*it).velocity / 100;
			}
			else
			{
				scaling = (4 * (*it).velocity / 100 * fade * fade);
			}

			mat4 mat(1.0f);
			mat = scale(mat, vec3(1.f, 1.0f / (float)noteCount, 1));// y = size of a note on display; y =
			mat = translate(mat, vec3(0, noteCount * (((float)(key - range[0]) / (float)noteCount * 2) -1), 0));// y = -5 +5
			mat = scale(mat, vec3(1.f, scaling, 1.f));

			shader.use();
			shader.setMat4("matrix", mat);
			shader.setVec4("color", vec4(0, 0, 1, opacity));
			quad->draw();
		}
}

smf::MidiEvent* Visualization::NewNote()
{
	//goes through the array of midi events stating from last event
	for (nEvent; nEvent < track->getEventCount(); nEvent++)
	{
		smf::MidiEvent* currentEvent = &(track->getEvent(nEvent));
		//is it "note on" event?
		if (currentEvent->isNoteOn() || currentEvent->isNoteOff())
		{
			//check if the time for this note has come
			if (currentEvent->seconds * CLOCKS_PER_SEC < now())
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

//Return time
int Visualization::now()
{
	return clock() - startTime;
}
