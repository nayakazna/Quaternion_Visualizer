#pragma once
#include <array>
#include <cmath>
#include <stdexcept>
#include "Vector3.hpp"

namespace math {
    template<typename T>
    class Matrix3 {
    public:
        // ctor
        Matrix3() : data{} {
            for (int i = 0; i < 9; ++i) {
                data[i] = (i % 4 == 0) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
        explicit Matrix3(const std::array<T, 9>& arr) : data(arr) {}

        T& operator()(int row, int col) {
            return data[row * 3 + col];
        }
        const T& operator()(int row, int col) const {
            return data[row * 3 + col];
        }

        // operator overloading
        Matrix3 operator*(const Matrix3& other) const {
            Matrix3 result;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result(i, j) = static_cast<T>(0);
                    for (int k = 0; k < 3; ++k) {
                        result(i, j) += (*this)(i, k) * other(k, j);
                    }
                }
            }
            return result;
        }

        Vector3<T> operator*(const Vector3<T>& v) const {
            return Vector3<T>(
                (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z,
                (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z,
                (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z
            );
        }

        Matrix3 transpose() const {
            Matrix3 result;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result(i, j) = (*this)(j, i);
                }
            }
            return result;
        }

    private:
        std::array<T, 9> data;
    };
} // namespace math
