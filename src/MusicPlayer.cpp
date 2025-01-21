#include "../include/fish_game/MusicPlayer.hpp"
#include <SDL2/SDL_mixer.h>

MusicPlayer::MusicPlayer() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}

	this->CombatMusic = Mix_LoadMUS("../../music/Fight.mp3");
	this->LobbyMusic = Mix_LoadMUS("../../music/Lobby.mp3");
	this->ShootSound = Mix_LoadWAV("../../music/shoot.wav");
	this->SplashSound = Mix_LoadWAV("../../music/splash.wav");
	this->EquipSound = Mix_LoadWAV("../../music/equip.wav");

	if (!(this->CombatMusic && this->LobbyMusic && this->ShootSound && this->SplashSound)) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

MusicPlayer::~MusicPlayer() {
	Mix_FreeMusic(this->CombatMusic);
	Mix_FreeMusic(this->LobbyMusic);
	Mix_FreeChunk(this->ShootSound);
	Mix_FreeChunk(this->SplashSound);
	Mix_FreeChunk(this->EquipSound);
	Mix_CloseAudio();
	SDL_Quit();
}

void MusicPlayer::playLobbyMusic() {
	if (Mix_PlayMusic(LobbyMusic, -1) == -1) {
		std::cerr << "Failed to play lobby music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::playCombatMusic() {
	if (Mix_PlayMusic(CombatMusic, -1) == -1) {
		std::cerr << "Failed to play combat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::playShootSound() {
	if (Mix_PlayChannel(-1, ShootSound, 0) == -1) {
		std::cerr << "Failed to play shoot sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::playSplashSound() {
	if (Mix_PlayChannel(-1, SplashSound, 0) == -1) {
		std::cerr << "Failed to play splash sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::playEquipSound() {
	if (Mix_PlayChannel(-1, EquipSound, 0) == -1) {
		std::cerr << "Failed to play splash sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

void MusicPlayer::stopMusic() {
	Mix_HaltMusic();
}

void MusicPlayer::setMusicVolume(int volume) {
	Mix_VolumeMusic(volume);
}
