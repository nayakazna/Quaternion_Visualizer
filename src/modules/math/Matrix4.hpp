#pragma once
#include <array>
#include <cmath>
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace math {
    template<typename T>
    class Matrix4 {
    public:
        using ValueType = T;
        using ArrayType = std::array<T, 16>;

        // ctor (matriks identitas)
        Matrix4() : data{} {
            for (int i = 0; i < 16; ++i) {
                data[i] = (i % 5 == 0) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
        // ctor (dari array)
        explicit Matrix4(const ArrayType& arr) : data(arr) {}
        // cctor
        Matrix4(const Matrix4& other) : data(other.data) {}
        
        // assignment
        T& operator()(int row, int col) {
            return data[row * 4 + col];
        }
        const T& operator()(int row, int col) const {
            return data[row * 4 + col];
        }

        // getter-setter
        ArrayType getData() const {
            return data;
        }
        ValueType getAt(int x, int y) const {
            if (x < 0 || x >= 4 || y < 0 || y >= 4) {
                throw std::out_of_range("[Matrix4] indeks di luar jangkauan.");
            }
            return data[x * 4 + y];
        }
        void setData(const ArrayType& arr) {
            data = arr;
        }
        void setAt(int x, int y, ValueType value) {
            if (x < 0 || x >= 4 || y < 0 || y >= 4) {
                throw std::out_of_range("[Matrix4] indeks di luar jangkauan.");
            }
            data[x * 4 + y] = value;
        }

        // operator overloading
        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result(i, j) = static_cast<T>(0);
                    for (int k = 0; k < 4; ++k) {
                        result(i, j) += this->operator()(i, k) * other(k, j);
                    }
                }
            }
            return result;
        }
        Matrix4& operator*=(const Matrix4& other) {
            *this = *this * other;
            return *this;
        }

        Matrix4 operator+(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = this->data[i] + other.data[i];
            }
            return result;
        }
        Matrix4& operator+=(const Matrix4& other) {
            for (int i = 0; i < 16; ++i) {
                this->data[i] += other.data[i];
            }
            return *this;
        }

        Matrix4 operator-(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = this->data[i] - other.data[i];
            }
            return result;
        }
        Matrix4& operator-=(const Matrix4& other) {
            for (int i = 0; i < 16; ++i) {
                this->data[i] -= other.data[i];
            }
            return *this;
        }

        Matrix4 operator*(T scalar) const {
            Matrix4 result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = this->data[i] * scalar;
            }
            return result;
        }
        Matrix4& operator*=(T scalar) {
            for (int i = 0; i < 16; ++i) {
                this->data[i] *= scalar;
            }
            return *this;
        }

        Matrix4 operator/(T scalar) const {
            if (scalar == static_cast<T>(0)) {
                throw std::runtime_error("[Matrix4] pembagian dengan nol.");
            }
            Matrix4 result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = this->data[i] / scalar;
            }
            return result;
        }
        Matrix4& operator/=(T scalar) {
            if (scalar == static_cast<T>(0)) {
                throw std::runtime_error("[Matrix4] pembagian dengan nol.");
            }
            for (int i = 0; i < 16; ++i) {
                this->data[i] /= scalar;
            }
            return *this;
        }

        bool operator==(const Matrix4& other) const {
            return data == other.data;
        }
        bool operator!=(const Matrix4& other) const {
            return !(*this == other);
        }

    Vector4<T> operator*(const Vector4<T>& vector) const {
        Vector4<T> result;
        result.x() = (*this)(0, 0) * vector.x() + (*this)(0, 1) * vector.y() + (*this)(0, 2) * vector.z() + (*this)(0, 3) * vector.w();
        result.y() = (*this)(1, 0) * vector.x() + (*this)(1, 1) * vector.y() + (*this)(1, 2) * vector.z() + (*this)(1, 3) * vector.w();
        result.z() = (*this)(2, 0) * vector.x() + (*this)(2, 1) * vector.y() + (*this)(2, 2) * vector.z() + (*this)(2, 3) * vector.w();
        result.w() = (*this)(3, 0) * vector.x() + (*this)(3, 1) * vector.y() + (*this)(3, 2) * vector.z() + (*this)(3, 3) * vector.w();
        return result;
    }

    // operasi thd matriksnya sendiri
    Matrix4 transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = this->operator()(j, i);
            }
        }
        return result;
    }


    Matrix4 inverse() const {
        T det;
        Matrix4 result;

        result.data[0] = data[5]  * data[10] * data[15] - 
                         data[5]  * data[11] * data[14] - 
                         data[9]  * data[6]  * data[15] + 
                         data[9]  * data[7]  * data[14] +
                         data[13] * data[6]  * data[11] - 
                         data[13] * data[7]  * data[10];

        result.data[4] = -data[4]  * data[10] * data[15] + 
                          data[4]  * data[11] * data[14] + 
                          data[8]  * data[6]  * data[15] - 
                          data[8]  * data[7]  * data[14] - 
                          data[12] * data[6]  * data[11] + 
                          data[12] * data[7]  * data[10];

        result.data[8] = data[4]  * data[9] * data[15] - 
                         data[4]  * data[11] * data[13] - 
                         data[8]  * data[5] * data[15] + 
                         data[8]  * data[7] * data[13] + 
                         data[12] * data[5] * data[11] - 
                         data[12] * data[7] * data[9];

        result.data[12] = -data[4]  * data[9] * data[14] + 
                           data[4]  * data[10] * data[13] +
                           data[8]  * data[5] * data[14] - 
                           data[8]  * data[6] * data[13] - 
                           data[12] * data[5] * data[10] + 
                           data[12] * data[6] * data[9];

        result.data[1] = -data[1]  * data[10] * data[15] + 
                          data[1]  * data[11] * data[14] + 
                          data[9]  * data[2] * data[15] - 
                          data[9]  * data[3] * data[14] - 
                          data[13] * data[2] * data[11] + 
                          data[13] * data[3] * data[10];

        result.data[5] = data[0]  * data[10] * data[15] - 
                         data[0]  * data[11] * data[14] - 
                         data[8]  * data[2] * data[15] + 
                         data[8]  * data[3] * data[14] + 
                         data[12] * data[2] * data[11] - 
                         data[12] * data[3] * data[10];

        result.data[9] = -data[0]  * data[9] * data[15] + 
                          data[0]  * data[11] * data[13] + 
                          data[8]  * data[1] * data[15] - 
                          data[8]  * data[3] * data[13] - 
                          data[12] * data[1] * data[11] + 
                          data[12] * data[3] * data[9];

        result.data[13] = data[0]  * data[9] * data[14] - 
                          data[0]  * data[10] * data[13] - 
                          data[8]  * data[1] * data[14] + 
                          data[8]  * data[2] * data[13] + 
                          data[12] * data[1] * data[10] - 
                          data[12] * data[2] * data[9];

        result.data[2] = data[1]  * data[6] * data[15] - 
                         data[1]  * data[7] * data[14] - 
                         data[5]  * data[2] * data[15] + 
                         data[5]  * data[3] * data[14] + 
                         data[13] * data[2] * data[7] - 
                         data[13] * data[3] * data[6];

        result.data[6] = -data[0]  * data[6] * data[15] + 
                          data[0]  * data[7] * data[14] + 
                          data[4]  * data[2] * data[15] - 
                          data[4]  * data[3] * data[14] - 
                          data[12] * data[2] * data[7] + 
                          data[12] * data[3] * data[6];

        result.data[10] = data[0]  * data[5] * data[15] - 
                          data[0]  * data[7] * data[13] - 
                          data[4]  * data[1] * data[15] + 
                          data[4]  * data[3] * data[13] + 
                          data[12] * data[1] * data[7] - 
                          data[12] * data[3] * data[5];

        result.data[14] = -data[0]  * data[5] * data[14] + 
                           data[0]  * data[6] * data[13] + 
                           data[4]  * data[1] * data[14] - 
                           data[4]  * data[2] * data[13] - 
                           data[12] * data[1] * data[6] + 
                           data[12] * data[2] * data[5];

        result.data[3] = -data[1] * data[6] * data[11] + 
                          data[1] * data[7] * data[10] + 
                          data[5] * data[2] * data[11] - 
                          data[5] * data[3] * data[10] - 
                          data[9] * data[2] * data[7] + 
                          data[9] * data[3] * data[6];

        result.data[7] = data[0] * data[6] * data[11] - 
                         data[0] * data[7] * data[10] - 
                         data[4] * data[2] * data[11] + 
                         data[4] * data[3] * data[10] + 
                         data[8] * data[2] * data[7] - 
                         data[8] * data[3] * data[6];

        result.data[11] = -data[0] * data[5] * data[11] + 
                           data[0] * data[7] * data[9] + 
                           data[4] * data[1] * data[11] - 
                           data[4] * data[3] * data[9] - 
                           data[8] * data[1] * data[7] + 
                           data[8] * data[3] * data[5];

        result.data[15] = data[0] * data[5] * data[10] - 
                          data[0] * data[6] * data[9] - 
                          data[4] * data[1] * data[10] + 
                          data[4] * data[2] * data[9] + 
                          data[8] * data[1] * data[6] - 
                          data[8] * data[2] * data[5];

        det = data[0] * result.data[0] + data[1] * result.data[4] + data[2] * result.data[8] + data[3] * result.data[12];

        if (det == static_cast<T>(0)) {
            return Matrix4();
        }

        det = static_cast<T>(1) / det;

        for (int i = 0; i < 16; ++i) {
            result.data[i] *= det;
        }

        return result;
    }

    // metod statis (biar enak)
    static Matrix4 identity() {
        return Matrix4();
    }

    static Matrix4 translation(const Vector3<T>& v) {
        Matrix4 result = identity();
        result(0, 3)= v.x;
        result(1, 3)= v.y;
        result(2, 3)= v.z;
        return result;
    }

    static Matrix4 scale(const Vector3<T>& s) {
        Matrix4 result = identity();
        result(0, 0) = s.x;
        result(1, 1) = s.y;
        result(2, 2) = s.z;
        return result;
    }

    static Matrix4 fromQuaternion(const Quaternion<T>& q) {
        Matrix4 result;
        T x2 = q.x * q.x;
        T y2 = q.y * q.y;
        T z2 = q.z * q.z;
        T xy = q.x * q.y;
        T xz = q.x * q.z;
        T yz = q.y * q.z;
        T wx = q.w * q.x;
        T wy = q.w * q.y;
        T wz = q.w * q.z;
        
        result(0, 0) = static_cast<T>(1) - static_cast<T>(2) * (y2 + z2);
        result(0, 1) = static_cast<T>(2) * (xy - wz);
        result(0, 2) = static_cast<T>(2) * (xz + wy);
        result(0, 3) = static_cast<T>(0);
        result(1, 0) = static_cast<T>(2) * (xy + wz);
        result(1, 1) = static_cast<T>(1) - static_cast<T>(2) * (x2 + z2);
        result(1, 2) = static_cast<T>(2) * (yz - wx);
        result(1, 3) = static_cast<T>(0);
        result(2, 0) = static_cast<T>(2) * (xz - wy);
        result(2, 1) = static_cast<T>(2) * (yz + wx);
        result(2, 2) = static_cast<T>(1) - static_cast<T>(2) * (x2 + y2);
        result(2, 3) = static_cast<T>(0);
        result(3, 0) = static_cast<T>(0);
        result(3, 1) = static_cast<T>(0);
        result(3, 2) = static_cast<T>(0);
        result(3, 3) = static_cast<T>(1);
        return result;
    }

    static Matrix4 lookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up) {
        Vector3<T> f = (center - eye).normalize();
        Vector3<T> s = f.cross(up).normalize();
        Vector3<T> u = s.cross(f);

        Matrix4 result;
        // 🚀 FIXED: Standard OpenGL lookAt matrix layout
        result(0, 0) = s.x;
        result(0, 1) = s.y;
        result(0, 2) = s.z;
        result(0, 3) = -s.dot(eye);  // 🔥 Translation di column 3!
        
        result(1, 0) = u.x;
        result(1, 1) = u.y;
        result(1, 2) = u.z;
        result(1, 3) = -u.dot(eye);  // 🔥 Translation di column 3!
        
        result(2, 0) = -f.x;
        result(2, 1) = -f.y;
        result(2, 2) = -f.z;
        result(2, 3) = f.dot(eye);   // 🔥 Translation di column 3!
        
        result(3, 0) = static_cast<T>(0);
        result(3, 1) = static_cast<T>(0);
        result(3, 2) = static_cast<T>(0);
        result(3, 3) = static_cast<T>(1);
        return result;
    }

    static Matrix4 perspective(T fovY, T aspect, T nearZ, T farZ) {
        Matrix4 result{};
        T tanHalfFovY = std::tan(fovY / static_cast<T>(2));
        T f = static_cast<T>(1) / tanHalfFovY;

        result(0, 0) = f / aspect;
        result(1, 1) = f;
        result(2, 2) = (farZ + nearZ) / (nearZ - farZ);
        result(2, 3) = (static_cast<T>(2) * farZ * nearZ) / (nearZ - farZ);
        result(3, 2) = static_cast<T>(-1);
        result(3, 3) = static_cast<T>(0);

        return result;
    }
    
    private:
        ArrayType data;
    };
} // namespace math
