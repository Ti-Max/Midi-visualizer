#include "MidiFile.h"

bool MidiFile::ParseFile(const std::string& file)
{
    //Open file
    std::ifstream ifs;
    ifs.open(file, std::fstream::in | std::fstream::binary);
	if (!ifs.is_open())
	{
		std::cout << "====Failed to open file====" << std::endl;
		return false;
	}

	// Helper Utilities ====================

		// Swaps byte order of 32-bit integer
	auto Swap32 = [](uint32_t n)
	{
		return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
	};

	// Swaps byte order of 16-bit integer
	auto Swap16 = [](uint16_t n)
	{
		return ((n >> 8) | (n << 8));
	};

	// Reads nLength bytes form file stream, and constructs a text string
	auto ReadString = [&ifs](uint32_t nLength)
	{
		std::string s;
		for (uint32_t i = 0; i < nLength; i++) s += ifs.get();
		return s;
	};

	auto ReadValue = [&ifs]()
	{
		uint32_t nValue = 0;
		uint8_t nByte = 0;

		// Read byte
		nValue = ifs.get();

		// Check MSB, if set, more bytes need reading
		if (nValue & 0x80)
		{
			// Extract bottom 7 bits of read byte
			nValue &= 0x7F;
			do
			{
				// Read next byte
				nByte = ifs.get();

				// Construct value by setting bottom 7 bits, then shifting 7 bits
				nValue = (nValue << 7) | (nByte & 0x7F);
			} while (nByte & 0x80); // Loop whilst read byte MSB is 1
		}

		// Return final construction (always 32-bit unsigned integer internally)
		return nValue;
	};
	uint16_t n16 = 0;
	uint32_t n32 = 0;
	// Read MIDI Header (Fixed Size)
	ifs.read((char*)&n32, sizeof(uint32_t));
	uint32_t nFileID = Swap32(n32);
	ifs.read((char*)&n32, sizeof(uint32_t));
	uint32_t nHeaderLength = Swap32(n32);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nFormat = Swap16(n16);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nTrackChunks = Swap16(n16);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nDivision = Swap16(n16);


	for (uint16_t nChunk = 0; nChunk < nTrackChunks; nChunk++)
	{
		std::cout << "===== NEW TRACK" << std::endl;
		// Read Track Header
		ifs.read((char*)&n32, sizeof(uint32_t));
		uint32_t nTrackID = Swap32(n32);
		ifs.read((char*)&n32, sizeof(uint32_t));
		uint32_t nTrackLength = Swap32(n32);

		bool bEndOfTrack = false;

		while (!ifs.eof() && !bEndOfTrack)
		{
			uint32_t nStatusTimeDelta = 0;
			uint8_t nStatus = 0;
			//delta time
			nStatusTimeDelta = ReadValue();
			//status byte
			nStatus = ifs.get();

		}
	}


    return false;
}
