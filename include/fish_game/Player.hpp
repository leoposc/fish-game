#pragma once

inline int generateSpriteID() {
	static int id = 0;
	return id++ % 6;
}

class Player {

  public:
	Player(int entity_id);

  private:
	int score = 0;
	int entity_id;
	int sprite_id;
};
