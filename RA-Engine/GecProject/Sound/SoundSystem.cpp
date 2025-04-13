// Includes:
#include "SoundSystem.h"

#include <cassert>
#include <iostream>

// Singleton:
// Creation, Getting, Destroying
SoundSystem* SoundSystem::m_instance = nullptr;
void SoundSystem::createInstance(void)
{
	if (m_instance == nullptr)
	{
		m_instance = new SoundSystem();
	}
}

SoundSystem& SoundSystem::getInstance(void)
{
	// Return M_instance reference
	assert(m_instance != nullptr);
	return *m_instance;
}

// Removing Instance of Singleton
void SoundSystem::destroyInstance(void)
{
	delete m_instance;
	m_instance = nullptr;
}

// Member Functions:

// Stops the sound channels from playing their audio
void SoundSystem::stopAllSounds(void)
{
	// Stopping Sounds:
	for (int i = 0; i < 4; i = i + 1)
	{
		m_soundChannels[i].stop();
	}
}

// stops the music channels from streaming their audio
void SoundSystem::stopAllMusic(void)
{
	// Stopping Music:
	for (int i = 0; i < 4; i = i + 1)
	{
		m_musicChannels[i].stop();
	}
}

// Stops Both Sounds and Music Channels
void SoundSystem::stopAll(void)
{
	stopAllSounds();
	stopAllMusic();
}

// Clears the data from the sound buffer map
// will clear all sound channels
void SoundSystem::clearSounds(void)
{
	stopAllSounds();
	m_sounds.clear();
}

// Sound and Music:
// Attempts to load a sound from the given file and then add it to the sound map
bool SoundSystem::loadSound(const std::string& a_soundFilePath)
{
	// Checking if the sound already exists:
	auto filepathKey = m_sounds.find(a_soundFilePath);
	if (filepathKey != m_sounds.end())
	{
		// Sound is already loaded in map:
		return true;
	}


	// Sound doesn't already exist
	// Load the sound:
	sf::SoundBuffer soundBuffer;
	if (!soundBuffer.loadFromFile(a_soundFilePath))
	{
		// Unsuccessful load:
		#if defined(_DEBUG)
			std::cerr << "Cannot Load Sound from file : SoundSystem : LoadSound Function" << '\n';
		#endif

		return false;
	}

	// Successful Load:
	// Adding Sound to map
	m_sounds.emplace(a_soundFilePath, soundBuffer);
	return true;
}


// Checks if the sound channel is valid, 
// Loads the appropriate sound,
// Plays the sound:
// Will return true if successfully playing sound
bool SoundSystem::playSound(const std::string& a_soundFilepath, const int a_aSoundChannel)
{
	// Checking if sound channel is valid:
	// There are 4 sound channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_aSoundChannel < 4 && a_aSoundChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_aSoundChannel << " Does not exist: \n There are 4 sound channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	// Unsuccessful Load:
	if (!loadSound(a_soundFilepath))
	{
		return false;
	}

	// Passed Error Handling:
	// Playing Sounds:
	m_soundChannels[a_aSoundChannel].setBuffer(m_sounds[a_soundFilepath]);
	m_soundChannels[a_aSoundChannel].play();
	return true;
}

// Checks if the music channel is valid, 
// Checks if the file is valid
// streams the appropriate music file,
// Will return true if successfully playing sound
bool SoundSystem::playMusic(const std::string& a_musicFilepath, const int a_musicChannel, const bool a_loop)
{
	// Checking if sound channel is valid:
	// There are 4 sound channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_musicChannel < 4 && a_musicChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Music Channel : " << a_musicChannel << " Does not exist: \n There are 4 Music channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	// Checking if file is appropriate:
	// Failing To load file:
	if (!m_musicChannels[a_musicChannel].openFromFile(a_musicFilepath))
	{
		#if defined(_DEBUG)
			std::cerr << "Sound System : PlayMusic Function, Cannot load given music file." << '\n';
		#endif

		// cant load file
		return false; 
	}


	// Playing Music 
	m_musicChannels[a_musicChannel].play();
	m_musicChannels[a_musicChannel].setLoop(a_loop);

	return true;
}

// Stops the playing of the sound coming from the given sound channel
bool SoundSystem::stopSoundChannel(const int a_aSoundChannel)
{
	// Checking if sound channel is valid:
	// There are 4 sound channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_aSoundChannel < 4 && a_aSoundChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_aSoundChannel << " Does not exist: \n There are 4 Sound channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}


	m_soundChannels[a_aSoundChannel].stop();
	return true;
}

// Stops the streaming of music from the given music channel
bool SoundSystem::stopMusicChannel(const int a_musicChannel)
{
	// Checking if Music channel is valid:
	// There are 4 Music channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_musicChannel < 4 && a_musicChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Music Channel : " << a_musicChannel << " Does not exist: \n There are 4 Music channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	m_musicChannels[a_musicChannel].stop();
	return true;
}

// stops the playing of the sound in the given sound channel,
// then calls to play it again, re-using the current filepath
bool SoundSystem::restartSoundChannel(const int a_soundChannel)
{
	// Checking if sound channel is valid:
	// There are 4 sound channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_soundChannel < 4 && a_soundChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_soundChannel << " Does not exist: \n There are 4 Sound channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	// Validity Check
	// Returns the current sound buffer for the channel
	// Can be NULL - SFML Comment
	if (m_soundChannels[a_soundChannel].getBuffer() == nullptr)
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_soundChannel << " Does not have a current sound buffer and thus can't be restarted: \n : Sound system Restart Sound Channel Function" <<
				'\n';
		#endif

		return false;
	}

	// Checks if the given sound channel is actually playing something.
	if (m_soundChannels[a_soundChannel].getStatus() != sf::SoundSource::Status::Playing)
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_soundChannel << " is not current playing sounds and thus cannot be restarted: \n : Sound System restart sound channel function" <<
				'\n';
		#endif

		return false;
	}

	// Stopping the sound then playing it again:
	m_soundChannels[a_soundChannel].play();

	return true;
}

// stops streaming the current music channel then re-play the audio
bool SoundSystem::restartMusicChannel(const int a_musicChannel)
{
	// Checking if Music channel is valid:
	// There are 4 Music channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_musicChannel < 4 && a_musicChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Music Channel : " << a_musicChannel << " Does not exist: \n There are 4 Music channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	// Checks if the given music channel is actually playing something.
	if (m_musicChannels[a_musicChannel].getStatus() != sf::SoundSource::Status::Playing)
	{
		#if defined(_DEBUG)
			std::cerr << "Music Channel : " << a_musicChannel << " is not current streaming music and thus cannot be restarted: \n : Sound System restart music channel function" <<
				'\n';
		#endif

		return false;
	}

	// Re-playing music stream:
	m_musicChannels[a_musicChannel].play();
	return true;
}

// checks if the given sound channel's sound status is "playing"
bool SoundSystem::checkIfSoundChannelIsPlayingAudio(const int a_soundChannel) const
{
	// Checking if sound channel is valid:
	// There are 4 sound channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_soundChannel < 4 && a_soundChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Sound Channel : " << a_soundChannel << " Does not exist: \n There are 4 Sound channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	return m_soundChannels[a_soundChannel].getStatus() == sf::SoundSource::Status::Playing;
}

bool SoundSystem::checkIfMusicChannelIsPlayingAudio(const int a_aMusicChannel) const
{
	// Checking if Music channel is valid:
	// There are 4 Music channels (0, 1, 2, 3)
	// Range Check:
	if (!(a_aMusicChannel < 4 && a_aMusicChannel > -1))
	{
		#if defined(_DEBUG)
			std::cerr << "Music Channel : " << a_aMusicChannel << " Does not exist: \n There are 4 Music channels, indexed 0, 1, 2, and 3" <<
				'\n';
		#endif

		// not valid channel:
		return false;
	}

	return m_musicChannels[a_aMusicChannel].getStatus() == sf::SoundSource::Status::Playing;
}




