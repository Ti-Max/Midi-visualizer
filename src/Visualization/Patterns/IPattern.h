#pragma once
#include<iostream>
struct NoteInfo
{
	NoteInfo(int n_key, int n_velocity, float n_noteOnTime, float n_duration) : key(n_key), velocity(n_velocity), noteOnTime(n_noteOnTime), duration(n_duration) {}

	int key = 0;
	int velocity = 0;
	// Note On event in seconds
	float noteOnTime = 0;
	//Duration in secondts
	float duration = 0;
};

class ISettings 
{
public:
	virtual ~ISettings() {};
};
class IPattern
{
public:
	//Child has to delete settings
	virtual void SetUp(ISettings* setting) = 0;
	virtual void Draw(const NoteInfo &note, long visualizationTime) = 0;
	virtual ~IPattern(){};
};

