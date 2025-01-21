
#include "../include/fish_game/MusicPlayer.hpp"

MusicPlayer::MusicPlayer() : music(nullptr) {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

MusicPlayer::~MusicPlayer() {
	if (music) {
		Mix_FreeMusic(music);
	}
	Mix_CloseAudio();
	SDL_Quit();
}

bool MusicPlayer::loadMusic(const std::string &filePath) {
	music = Mix_LoadMUS(filePath.c_str());
	if (!music) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void MusicPlayer::playMusic(int loops) {
	if (Mix_PlayMusic(music, loops) == -1) {
		std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::stopMusic() {
	Mix_HaltMusic();
}
