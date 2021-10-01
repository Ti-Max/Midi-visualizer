#pragma once
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"
#include "MidiFile/Midifile.h"

class Visualization
{
public:
	//call this before main loop
	void Start(smf::MidiEventList* track);
	//call every frame
	void Draw();
private:
	int startTime = 0;
	int lastTick = 0;
	int nEvent = 0;
	bool firstNote = true;
	//this track will be visualized
	smf::MidiEventList* track;
	//----------------------------
	smf::MidiEvent* NewNote();
	//=============================
	//TEMPORAL render
	Meshes quadMesh;
	Shader shader;
	GL::VAO* test;
};

