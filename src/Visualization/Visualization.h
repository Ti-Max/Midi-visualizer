#pragma once
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"
#include "MidiFile/Midifile.h"
#include <memory>
using namespace glm;

struct NoteInfo
{
	NoteInfo(int n_key, int n_velocity, int n_noteOnTime, int n_duration): key(n_key), velocity(n_velocity), noteOnTime(n_noteOnTime), duration(n_duration){}
	
	int key = 0;
	int velocity = 0;
	// Note On event in seconds
	float noteOnTime = 0;
	//Duration in secondts
	float duration = 0;
};

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
	GL::VAO* quad;
	//current notes
	std::vector<const NoteInfo&> currentNotes;
};

