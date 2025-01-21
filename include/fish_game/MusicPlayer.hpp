#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

class MusicPlayer {
  public:
	MusicPlayer();
	~MusicPlayer();

	bool loadMusic(const std::string &filePath);
	void playMusic(int loops = -1); // -1 means loop indefinitely
	void stopMusic();

  private:
	Mix_Music *music;
};

#endif // MUSICPLAYER_HPP
