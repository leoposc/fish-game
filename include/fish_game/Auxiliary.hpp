#pragma once

#include <SDL2/SDL.h>
#include <cereal/archives/json.hpp>
#include <string>

template <class Archive>
void serialize(Archive &ar, SDL_Event &event) {
	// ar(event);
	ar(event.type, event.key.keysym.sym);
}

template <class Archive>
void serialize(Archive &ar, SDL_Rect &rect) {
	ar(CEREAL_NVP(rect.x), CEREAL_NVP(rect.y), CEREAL_NVP(rect.w), CEREAL_NVP(rect.h));
}

bool isValidIPv4(std::string ip);

// template <class Archive>
// void serialize(Archive &ar, FishEngine::Vector2D &vec) {
// 	ar(CEREAL_NVP(vec.getX()), CEREAL_NVP(vec.getY()));
// }