#include "Sound.hpp"

namespace Sound
{
	void LoadSound(short Sound)
	{
		if(Soundzustand == -1) // Wenn keine Soundkarte vorhanden raus...
			return;

		if(!sound_buffers[Sound].loadFromFile(Wav[Sound].Dateiname))
			sf::err() << "Couldn't load sound file '" << Wav[Sound].Dateiname << "'!" << std::endl;

		sound_players[Sound].setBuffer(sound_buffers[Sound]);
		sound_players[Sound].setVolume(static_cast<float>(Wav[Sound].Volume));
		sound_players[Sound].setLoop(Wav[Sound].Loop);
	}

	void PlaySound(short Sound, short Volume)
	{
		if((Sound == 0) || (Soundzustand <= 0) || (sound_players[Sound].getStatus() == sf::Sound::Playing))
			return;

		sound_players[Sound].play();
	}

	void StopSound(short Sound)
	{
		if((Sound == 0) || (Soundzustand <= 0))
			return;

		sound_players[Sound].stop();
	}

} // namespace Sound