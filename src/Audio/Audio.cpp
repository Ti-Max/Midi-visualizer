#include "Audio.h"

static Audio* _instance = nullptr;

Audio* Audio::get()
{
	init();
	return _instance;
}

void Audio::init(const char* deviceName)
{
	if (_instance == nullptr)
		_instance = new Audio;
	_instance->context.Init(deviceName);
}

SoundBuffer* Audio::addBuffer(SoundBuffer* buffer, const std::string& name)
{
	_instance->soundBuffers.emplace(name, buffer);
	return buffer;
}

SoundSource* Audio::addSource(SoundSource* source, const std::string& name)
{
	_instance->soundSources.emplace(name, source);
	return source;
}

void Audio::Play(const std::string& source, const std::string& buffer)
{
	//check if the source exist
	auto it_source = _instance->soundSources.find(source);
	if (it_source != _instance->soundSources.end())
	{
		if (buffer != "default")
		{
			//check if the buffer exist
			auto it_buffer = _instance->soundBuffers.find(buffer);
			if (it_buffer != _instance->soundBuffers.end())
			{
				it_source->second->Play(it_buffer->second);
			}
			else
			{
				std::cout << "Buffer does not exist: \"" << buffer << "\"\n";
			}
		}
		else
		{
			//plays default buffer
			it_source->second->Play();
		}
	}
	else
	{
		std::cout << "Source does not exist: \"" << source << "\"\n";
	}
}

bool Audio::isPaused(const std::string& source)
{
	return _instance->soundSources[source]->isPaused();
}

void Audio::Pause(const std::string& source)
{
	_instance->soundSources[source]->Pause();
}

Audio::Audio()
{
}


Audio::~Audio()
{
	for (auto buffer : soundBuffers) {
		delete buffer.second;
	}
	soundBuffers.clear();
	for (auto source : soundSources) {
		delete source.second;
	}
	soundSources.clear();
}
