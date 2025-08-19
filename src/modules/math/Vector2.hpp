#pragma once
#include <cmath>
#include <stdexcept>

namespace math {
    template<typename T>
    class Vector2 {
    public:
        T x, y;

        // ctor
        Vector2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}
        Vector2(T x, T y) : x(x), y(y) {}
        explicit Vector2(const Vector3<T>& v) : x(v.x), y(v.y) {}

        // operator overloading
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }
        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }
        Vector2 operator*(T scalar) const {
            return Vector2(x * scalar, y * scalar);
        }
        Vector2 operator/(T scalar) const {
            if (scalar == static_cast<T>(0)) {
                return Vector2();
            }
            T invScalar = static_cast<T>(1) / scalar;
            return Vector2(x * invScalar, y * invScalar);
        }

        T length() const {
            return static_cast<T>(std::sqrt(x * x + y * y));
        }
        T dot(const Vector2& other) const {
            return x * other.x + y * other.y;
        }
        Vector2 normalize() const {
            T len = length();
            if (len == static_cast<T>(0)) {
                return Vector2();
            }
            return (*this) / len;
        }

        // ngeconvert koordinat (yg udh ternormalisasi, -1 smpe 1) ke layar (pixel)
        Vector2 toScreenCoords(int screenWidth, int screenHeight) const {
            T newX = (x + static_cast<T>(1)) * static_cast<T>(0.5) * static_cast<T>(screenWidth);
            T newY = (static_cast<T>(1) - y) * static_cast<T>(0.5) * static_cast<T>(screenHeight);
            return Vector2(newX, newY);
        }
    };
} // namespace math
