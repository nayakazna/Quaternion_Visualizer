// Quaternion.hpp
#pragma once
#include <cmath>
#include <stdexcept>
#include "Vector3.hpp"

namespace math {
    template<typename T>
    class Quaternion {
    public:
        T w, x, y, z;

        // ctor
        Quaternion() : w(static_cast<T>(1)), x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}
        Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}

        // "cctor"
        static Quaternion fromAxisAngle(const Vector3<T>& axis, T angleRad) {
            T halfAngle = angleRad / static_cast<T>(2);
            T sinHalfAngle = std::sin(halfAngle);
            return Quaternion(std::cos(halfAngle), axis.x * sinHalfAngle, axis.y * sinHalfAngle, axis.z * sinHalfAngle);
        }

        Quaternion conjugate() const {
            return Quaternion(w, -x, -y, -z);
        }

        T length() const {
            return std::sqrt(w * w + x * x + y * y + z * z);
        }

        Quaternion normalize() const {
            T len = length();
            if (len == static_cast<T>(0)) {
                return Quaternion();
            }
            return Quaternion(w / len, x / len, y / len, z / len);
        }

        // operator overloading
        Quaternion operator*(const Quaternion& other) const {
            return Quaternion(
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
            );
        }
        
        // rotasi thd Vector3
        Vector3<T> rotate(const Vector3<T>& v) const {
            Quaternion<T> p(static_cast<T>(0), v.x, v.y, v.z);
            Quaternion<T> rotated_p = (*this) * p * this->conjugate();
            return Vector3<T>(rotated_p.x, rotated_p.y, rotated_p.z);
        }
    };
} // namespace math
