#pragma once
#include <map>
#include <SFML/Audio.hpp>
#include <assert.h>
class SoundHandler
{
private:
	static std::map<std::string, sf::SoundBuffer> soundBufferFiles;
	static std::map<std::string, sf::Sound> playableSounds;
	static SoundHandler* p_SoundPointer;
public:
	SoundHandler();
	static void LoadNecessarySoundsToBuffers();
	static void LoadNecessarySoundsToSFMLSounds();
	//void LoadAdditionalSounds();
	static void playSound(std::string const& soundName, int volume);
};