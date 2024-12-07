#pragma once

#include <iostream>

// TODO: implemtate template for Vector2D

namespace FishEngine {

class Vector2D {
	float x;
	float y;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(x, y);
	}

	Vector2D();
	Vector2D(float x, float y);

	float getX() const { return x; }
	float getY() const { return y; }
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }

	Vector2D &add(const Vector2D &vec);
	Vector2D &multiply(const Vector2D &vec);
	Vector2D &subtract(const Vector2D &vec);
	Vector2D &divide(const Vector2D &vec);

	Vector2D &operator+=(const Vector2D &vec);
	Vector2D &operator-=(const Vector2D &vec);
	Vector2D &operator*=(const Vector2D &vec);
	Vector2D &operator/=(const Vector2D &vec);

	template <typename T>
	Vector2D operator+(const T &t) const {
		return Vector2D(x + t, y + t);
	}

	Vector2D operator+(const Vector2D &vec) const { return Vector2D(x + vec.x, y + vec.y); }

	template <typename T>
	Vector2D operator-(const T &t) const {
		return Vector2D(x - t, y - t);
	}

	Vector2D operator-(const Vector2D &vec) const { return Vector2D(x - vec.x, y - vec.y); }

	template <typename T>
	Vector2D operator*(const T &t) const {
		return Vector2D(x * t, y * t);
	}

	Vector2D operator*(const Vector2D &vec) const { return Vector2D(x * vec.x, y * vec.y); }

	template <typename T>
	Vector2D operator/(const T &t) const {
		return Vector2D(x / t, y / t);
	}

	Vector2D operator/(const Vector2D &vec) const { return Vector2D(x / vec.x, y / vec.y); }

	std::ostream &operator<<(std::ostream &os);
};

// Vector2D operator+(const Vector2D &vec1, const Vector2D &vec2);
// Vector2D operator-(const Vector2D &vec1, const Vector2D &vec2);
// Vector2D operator*(const Vector2D &vec1, const Vector2D &vec2);
// Vector2D operator/(const Vector2D &vec) const;

} // namespace FishEngine