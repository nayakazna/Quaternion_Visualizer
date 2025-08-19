// Vector4.hpp
#pragma once
#include <array>
#include <stdexcept>
#include <cmath>

#include "Vector3.hpp"

namespace math {
    template<typename T>
    class Vector4 {
    public:
        // ctor
        Vector4() : data{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)} {}
        Vector4(T x, T y, T z, T w) : data{x, y, z, w} {}
        
        // ctor buat ngubah ke koordinat homogen
        explicit Vector4(const Vector3<T>& v, T w) : data{v.x, v.y, v.z, w} {}

        // Akses komponen
        T& x() { return data[0]; }
        const T& x() const { return data[0]; }
        T& y() { return data[1]; }
        const T& y() const { return data[1]; }
        T& z() { return data[2]; }
        const T& z() const { return data[2]; }
        T& w() { return data[3]; }
        const T& w() const { return data[3]; }

        // operator overloading
        Vector4 operator+(const Vector4& other) const {
            return Vector4(
                data[0] + other.data[0],
                data[1] + other.data[1],
                data[2] + other.data[2],
                data[3] + other.data[3]
            );
        }
        Vector4 operator-(const Vector4& other) const {
            return Vector4(
                data[0] - other.data[0],
                data[1] - other.data[1],
                data[2] - other.data[2],
                data[3] - other.data[3]
            );
        }
        Vector4 operator*(T scalar) const {
            return Vector4(
                data[0] * scalar,
                data[1] * scalar,
                data[2] * scalar,
                data[3] * scalar
            );
        }

    private:
        std::array<T, 4> data;
    };
} // namespace math
