#include "SoundHandler.h"
std::map<std::string, sf::SoundBuffer> SoundHandler::soundBufferFiles;
std::map<std::string, sf::Sound> SoundHandler::playableSounds;
SoundHandler* SoundHandler::p_SoundPointer = nullptr;

SoundHandler::SoundHandler()
{
	assert(p_SoundPointer == nullptr);
	p_SoundPointer = this;
}

void SoundHandler::LoadNecessarySoundsToBuffers()
{
	sf::SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	soundBufferFiles.insert({ "hit", hitBuffer });

	sf::SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	soundBufferFiles.insert({ "splat", splatBuffer });

	sf::SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	soundBufferFiles.insert({ "shoot", shootBuffer });

	sf::SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	soundBufferFiles.insert({ "reload", reloadBuffer });

	sf::SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	soundBufferFiles.insert({ "reloadFailed", reloadFailedBuffer });

	sf::SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	soundBufferFiles.insert({ "powerup", powerupBuffer });

	sf::SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	soundBufferFiles.insert({ "pickup", pickupBuffer });
}

void SoundHandler::LoadNecessarySoundsToSFMLSounds()
{
	sf::Sound hit;
	hit.setBuffer(soundBufferFiles.at("hit"));
	playableSounds.insert({ "hit", hit });

	sf::Sound splat;
	splat.setBuffer(soundBufferFiles.at("splat"));
	playableSounds.insert({ "splat", splat });

	sf::Sound shoot;
	shoot.setBuffer(soundBufferFiles.at("shoot"));
	playableSounds.insert({ "shoot", shoot });

	sf::Sound reload;
	reload.setBuffer(soundBufferFiles.at("reload"));
	playableSounds.insert({ "reload", reload });

	sf::Sound reloadFailed;
	reloadFailed.setBuffer(soundBufferFiles.at("reloadFailed"));
	playableSounds.insert({ "reloadFailed", reloadFailed });

	sf::Sound powerup;
	powerup.setBuffer(soundBufferFiles.at("powerup"));
	playableSounds.insert({ "powerup", powerup });

	sf::Sound pickup;
	pickup.setBuffer(soundBufferFiles.at("pickup"));
	playableSounds.insert({ "pickup", pickup });

}

void SoundHandler::playSound(std::string const& soundName, int volume=15)
{
	if (soundBufferFiles.contains(soundName))
	{
	auto& a = p_SoundPointer->playableSounds.at(soundName);
	a.setVolume(volume);
	a.play();
	}
}
