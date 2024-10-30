#include "../include/fish_game/Vector2D.hpp"

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D &Vector2D::add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D &Vector2D::multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

Vector2D &Vector2D::subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D &Vector2D::divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return this->add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return this->subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return this->multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return this->divide(vec);
}

// Vector2D Vector2D::operator+(const Vector2D &vec) const
// {
//     return Vector2D(this->x + vec.x, this->y + vec.y);
// }

// Vector2D Vector2D::operator-(const Vector2D &vec) const
// {
//     return Vector2D(this->x - vec.x, this->y - vec.y);
// }

// Vector2D Vector2D::operator*(const Vector2D &vec) const
// {
//     return Vector2D(this->x * vec.x, this->y * vec.y);
// }

// Vector2D Vector2D::operator/(const Vector2D &vec) const
// {
//     return Vector2D(this->x / vec.x, this->y / vec.y);
// }

std::ostream &Vector2D::operator<<(std::ostream &os)
{
    os << "(" << this->x << ", " << this->y << ")";
    return os;
}
