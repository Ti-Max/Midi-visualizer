#include "SoundContext.h"

SoundContext::SoundContext(const char* deviceName)
{
	Init(deviceName);
}



void SoundContext::Init(const ALCchar* deviceName)
{
	p_ALCDevice = alcOpenDevice(deviceName); // nullptr = get default device
	if (!p_ALCDevice)
		std::cout<<"failed to get sound device"<<std::endl;

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if (!p_ALCContext)
		std::cout << "Failed to set sound context" << std::endl;

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		std::cout << "failed to make context current" << std::endl;

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}
SoundContext::~SoundContext()
{
	if (!alcMakeContextCurrent(nullptr))
		std::cout << "failed to set context to nullptr" << std::endl;

	alcDestroyContext(p_ALCContext);
	//if (p_ALCContext)
	//	std::cout << "failed to unset during close" << std::endl;

	if (!alcCloseDevice(p_ALCDevice))
		std::cout << "failed to close sound device" << std::endl;
}