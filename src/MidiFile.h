#pragma once
#include <iostream>
#include<fstream>

using namespace std;
class MidiFile
{
public:
	MidiFile()
	{
	}
	MidiFile(const std::string& file)
	{
		ParseFile(file);
	}
	bool ParseFile(const std::string& file);
};

