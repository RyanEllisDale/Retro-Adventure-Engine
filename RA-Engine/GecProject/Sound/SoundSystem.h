// Sound system with 4 sound and music files,
// uses sfml/audio.hpp to build a safe minimal sound singleton

#pragma once

// Includes:
#include <SFML/audio.hpp>
#include <unordered_map>
#include <string>

// Global Reference:
#define SOUND_SYSTEM SoundSystem::getInstance() 

// Handles The Sound System For The Game:
// Has 8 sound Channels:
	// 4 for music 
	// 4 for audio
class SoundSystem
{
private:
	// Private Variables:
	static SoundSystem* m_instance;

	std::unordered_map<std::string, sf::SoundBuffer> m_sounds;
	sf::Music m_musicChannels[4];
	sf::Sound m_soundChannels[4];

	// Construction:
	SoundSystem(void) = default;
	~SoundSystem(void) = default;

public:
	// Singleton:
	static void createInstance(void);
	static SoundSystem& getInstance(void);
	static void destroyInstance(void);

	// Member Functions:
	void stopAllSounds(void);
	void stopAllMusic(void);
	void stopAll(void);
	void clearSounds(void);
	bool loadSound(const std::string& a_soundFilePath);
	bool playSound(const std::string& a_soundFilepath, const int a_aSoundChannel);
	bool playMusic(const std::string& a_musicFilepath, const int a_musicChannel, const bool a_loop = false);
	bool stopSoundChannel(const int a_aSoundChannel);
	bool stopMusicChannel(const int a_musicChannel);
	bool restartSoundChannel(const int a_soundChannel);
	bool restartMusicChannel(const int a_musicChannel);
	bool checkIfSoundChannelIsPlayingAudio(const int a_soundChannel) const;
	bool checkIfMusicChannelIsPlayingAudio(const int a_aMusicChannel) const;
};

