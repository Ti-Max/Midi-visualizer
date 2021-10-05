//Seems like Audio goes faster. FIX IT
#include "Visualization.h"

#include <ctime>
#include<iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

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

void Visualization::Draw()
{
	if (startTime == 0) 
	{
		std::cout << "Call function Start() before calling Draw!!\n";
		return;
	}


	//Now we are checking if we got any new notes
	smf::MidiEvent* note;
	while((note = NewNote()) != nullptr)
	{
		if (note->isNoteOff())
		{
			matricies.erase(note->getKeyNumber());
		}
		else
		{
			int key = note->getKeyNumber();//1 - 127
			print(key);
			//compress to 50 - 100
			float yPos = ((key < 50) ? 50 : (key > 100) ? 100 : key) - 50;
			//make it to range -10 +10
			yPos = yPos / 50 * 40 - 20;

			std::shared_ptr<mat4> mat = std::make_shared<mat4>(1.0f);
			*mat = scale(*mat, vec3(0.5, 0.05, 1));// y =  1/40 of the screen  
			*mat = translate(*mat, vec3(0, yPos, 0));// y = -10 +10

			matricies.insert({ key, mat });
		}
	}
	for (auto mat : matricies)
	{
		if (!mat.second)
			print("errroorrooeroro");
		shader.use();
		mat4 m = *(mat.second);
		shader.setMat4("matrix", m);
		quad->draw();
	}
}

smf::MidiEvent* Visualization::NewNote()
{
	//goes through the array of midi events stating from last event
	for (nEvent; nEvent < track->getEventCount(); nEvent++)
	{
		//smf::MidiEvent* currentEvent = &(track->getEvent(nEvent));
		//is it "note on" event?
		if (track->getEvent(nEvent).isNoteOn() || track->getEvent(nEvent).isNoteOff())
		{
			//check if the time for this note has come
			int now = clock() - startTime;
			if (track->getEvent(nEvent).seconds * CLOCKS_PER_SEC < now)
			{
				//check if two notes pressed at the same time (optional)
				//if (track->getEvent(nEvent).tick != lastTick || firstNote)
				{

					lastTick = track->getEvent(nEvent).tick;
					std::cout << "in loop: " << now << "\t sec: " << track->getEvent(nEvent).seconds << std::endl;
					//if (firstNote)
					//	firstNote = false;

					return &(track->getEvent(nEvent++));
				}
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
