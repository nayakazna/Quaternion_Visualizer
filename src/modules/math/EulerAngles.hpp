#pragma once
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include <cmath>

namespace math {
    
    template<typename T>
    class EulerAngles {
    public:
        
        static Matrix4<T> fromZYX(T alpha, T beta, T gamma) {
            
            T alphaRad = alpha * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T betaRad = beta * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T gammaRad = gamma * static_cast<T>(M_PI) / static_cast<T>(180.0);
            
            
            T ca = std::cos(alphaRad), sa = std::sin(alphaRad);
            T cb = std::cos(betaRad),  sb = std::sin(betaRad);
            T cg = std::cos(gammaRad), sg = std::sin(gammaRad);
            
            
            Matrix4<T> result;
            
            result(0, 0) = ca * cb;
            result(0, 1) = ca * sb * sg - sa * cg;
            result(0, 2) = ca * sb * cg + sa * sg;
            result(0, 3) = static_cast<T>(0);
            
            result(1, 0) = sa * cb;
            result(1, 1) = sa * sb * sg + ca * cg;
            result(1, 2) = sa * sb * cg - ca * sg;
            result(1, 3) = static_cast<T>(0);
            
            result(2, 0) = -sb;
            result(2, 1) = cb * sg;
            result(2, 2) = cb * cg;
            result(2, 3) = static_cast<T>(0);
            
            result(3, 0) = static_cast<T>(0);
            result(3, 1) = static_cast<T>(0);
            result(3, 2) = static_cast<T>(0);
            result(3, 3) = static_cast<T>(1);
            
            return result;
        }
        
        
        static void getRotationAxes(T alpha, T beta, T gamma, 
                                   Vector3<T>& zAxis, Vector3<T>& yAxis, Vector3<T>& xAxis) {
            
            zAxis = Vector3<T>(0, 0, 1);
            
            
            T alphaRad = alpha * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T ca = std::cos(alphaRad), sa = std::sin(alphaRad);
            yAxis = Vector3<T>(-sa, ca, 0);
            
            
            T betaRad = beta * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T cb = std::cos(betaRad), sb = std::sin(betaRad);
            
            T xAxisX = ca * cb;
            T xAxisY = sa * cb;
            T xAxisZ = -sb;
            xAxis = Vector3<T>(xAxisX, xAxisY, xAxisZ);
        }
    };
    
    template<typename T>
    class TaitBryanAngles {
    public:
        
        static Matrix4<T> fromYawPitchRoll(T yaw, T pitch, T roll) {
            
            T yawRad = yaw * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T pitchRad = pitch * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T rollRad = roll * static_cast<T>(M_PI) / static_cast<T>(180.0);
            
            
            T cy = std::cos(yawRad),   sy = std::sin(yawRad);
            T cp = std::cos(pitchRad), sp = std::sin(pitchRad);
            T cr = std::cos(rollRad),  sr = std::sin(rollRad);
            
            
            Matrix4<T> result;
            
            result(0, 0) = cy * cp;
            result(0, 1) = cy * sp * sr - sy * cr;
            result(0, 2) = cy * sp * cr + sy * sr;
            result(0, 3) = static_cast<T>(0);
            
            result(1, 0) = sy * cp;
            result(1, 1) = sy * sp * sr + cy * cr;
            result(1, 2) = sy * sp * cr - cy * sr;
            result(1, 3) = static_cast<T>(0);
            
            result(2, 0) = -sp;
            result(2, 1) = cp * sr;
            result(2, 2) = cp * cr;
            result(2, 3) = static_cast<T>(0);
            
            result(3, 0) = static_cast<T>(0);
            result(3, 1) = static_cast<T>(0);
            result(3, 2) = static_cast<T>(0);
            result(3, 3) = static_cast<T>(1);
            
            return result;
        }
        
        
        static void getRotationAxes(T yaw, T pitch, T roll,
                                   Vector3<T>& yawAxis, Vector3<T>& pitchAxis, Vector3<T>& rollAxis) {
            T yawRad = yaw * static_cast<T>(M_PI) / static_cast<T>(180.0);
            T pitchRad = pitch * static_cast<T>(M_PI) / static_cast<T>(180.0);
            
            T cy = std::cos(yawRad), sy = std::sin(yawRad);
            T cp = std::cos(pitchRad), sp = std::sin(pitchRad);
            
            
            yawAxis = Vector3<T>(0, 0, 1);
            
            
            pitchAxis = Vector3<T>(-sy, cy, 0);
            
            
            rollAxis = Vector3<T>(cy * cp, sy * cp, -sp);
        }
    };
    
}  // namespace math