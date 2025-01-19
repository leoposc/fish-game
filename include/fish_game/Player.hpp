#pragma once

inline int generateSpriteID() {
	static int id = 0;
	return id++ % 6;
}

class Player {

  public:
	Player(int entity_id);

	int getScore() const { return score; }

	void setScore(int newScore) { score = newScore; }

	// Getter and Setter for entity_id
	int getEntityId() const { return entity_id; }

	void setEntityId(int newEntityId) { entity_id = newEntityId; }

	// Getter and Setter for sprite_id
	int getSpriteId() const { return sprite_id; }

	void setSpriteId(int newSpriteId) { sprite_id = newSpriteId; }

  private:
	int score = 0;
	int entity_id;
	int sprite_id;
};
