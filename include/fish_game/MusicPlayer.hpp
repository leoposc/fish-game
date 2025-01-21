#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

class MusicPlayer {
  public:
	static MusicPlayer &getInstance() {
		static MusicPlayer instance;
		return instance;
	}

	bool loadMusic(const std::string &filePath);
	void stopMusic();

	void playLobbyMusic();
	void playCombatMusic();
	void playShootSound();
	void playSplashSound();
	void playEquipSound();
	void playJumpSound();
	void setMusicVolume(int volume); // New method to set music volume

  private:
	MusicPlayer();
	~MusicPlayer();

	// Delete copy constructor and assignment operator
	MusicPlayer(const MusicPlayer &) = delete;
	MusicPlayer &operator=(const MusicPlayer &) = delete;

	Mix_Music *LobbyMusic;
	Mix_Music *CombatMusic;
	Mix_Chunk *ShootSound;
	Mix_Chunk *SplashSound;
	Mix_Chunk *EquipSound;
	Mix_Chunk *JumpSound;
};

#endif // MUSICPLAYER_HPP
