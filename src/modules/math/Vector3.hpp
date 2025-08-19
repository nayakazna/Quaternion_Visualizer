#pragma once
#include <cmath>

namespace math {
    template<typename T>
    class Vector3 {
    public:
        T x, y, z;

        // ctor
        Vector3() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        // operator overloading
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator*(T scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        Vector3 operator/(T scalar) const {
            if (scalar == static_cast<T>(0)) {
                return Vector3();
            }
            T invScalar = static_cast<T>(1) / scalar;
            return Vector3(x * invScalar, y * invScalar, z * invScalar);
        }

        bool operator==(const Vector3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
        bool operator!=(const Vector3& other) const {
            return !(*this == other);
        }

        T length() const {
            return static_cast<T>(std::sqrt(x * x + y * y + z * z));
        }
        T dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }
        Vector3 cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }
        Vector3 normalize() const {
            T len = length();
            if (len == static_cast<T>(0)) {
                return Vector3();
            }
            return (*this) / len;
        }

    };
} // namespace math
