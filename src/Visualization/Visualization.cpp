#include "Visualization.h"

#include <ctime>
#include<iostream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
void Visualization::Start(smf::MidiEventList* track)
{
	this->track = track;

	quadMesh.load();
	test = quadMesh["quad"];

	shader.load("quadShader");
	shader.use();
	shader.setVec3("color", glm::vec3(0, 1, 0));

	std::srand(time(0));

	startTime = clock();

}

void Visualization::Draw()
{
	if (startTime == 0) 
	{
		std::cout << "Call function Start() before calling Draw!!\n";
		return;
	}

	//Now we are checking if we got any new notes
	smf::MidiEvent* note = NewNote();
	if (note != nullptr)
	{
		int key = note->getKeyNumber();
		glm::mat4 scale;// = glm::scale(glm::mat4(1.0f), glm::vec3(key * 0.01));
		glm::translate(scale, glm::vec3(key, 0, 0));
		shader.use();
		shader.setMat4("matrix", scale);
	}


	shader.use();
	test->draw();
}

smf::MidiEvent* Visualization::NewNote()
{
	//goes through the array of midi events stating from last event
	for (nEvent; nEvent < track->getEventCount(); nEvent++)
	{
		std::cout << "here\n";
		//smf::MidiEvent* currentEvent = &(track->getEvent(nEvent));
		//is it "note on" event?
		if (track->getEvent(nEvent).isNoteOn())
		{
			//check if the time for this note has come
			int now = clock() - startTime;
			if (track->getEvent(nEvent).seconds * CLOCKS_PER_SEC < now)
			{
				//check if two notes pressed at the same time (optional)
				if (track->getEvent(nEvent).tick != lastTick || firstNote)
				{

					lastTick = track->getEvent(nEvent).tick;
					std::cout << "in loop: " << now << "\t sec: " << track->getEvent(nEvent).seconds << std::endl;
					if (firstNote)
						firstNote = false;
					return &(track->getEvent(nEvent));
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
