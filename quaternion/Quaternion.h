#pragma once
#include <cmath>
#include <ostream>
#include "Vector.h"

template <typename T>
class QuaternionT {
public:
    Vector3T<T> v;
    T s;

    constexpr QuaternionT (T s, Vector3T<T> v) : v(std::move(v)), s(s)  { }

    static constexpr QuaternionT<T> Euler (const Vector3T<T>& pitch_yaw_roll) {
        return Euler(pitch_yaw_roll.x, pitch_yaw_roll.y, pitch_yaw_roll.z);
    }
    static constexpr QuaternionT<T> Euler (T pitch, T yaw, T roll) {
        const Vector3T<T> c (std::cos(pitch/2), std::cos(yaw/2), std::cos(roll/2));
        const Vector3T<T> s (std::sin(pitch/2), std::sin(yaw/2), std::sin(roll/2));
        return QuaternionT<T> (
            c.x*c.y*c.z + s.x*s.y*s.z,
            Vector3T<T> (
                s.x*c.y*c.z - c.x*s.y*s.z,
                c.x*s.y*c.z + s.x*c.y*s.z,
                c.x*c.y*s.z - s.x*s.y*c.z
            )
        );
    }
    static constexpr QuaternionT<T> Identity () {
        return QuaternionT<T> (1, {0, 0, 0});
    }

    static constexpr QuaternionT<T> Rotation (T angle, Vector3T<T> axis) {
        axis.Normalize();
        return RotationN(std::move(angle), std::move(axis));
    }

    static constexpr QuaternionT<T> RotationN (T angle, Vector3T<T> normalizedAxis) {
        const T halfAng = angle/2;
        return QuaternionT<T> (std::cos(halfAng), std::sin(halfAng) * normalizedAxis);
    }

    // // Alternative multiplication implementation, seems to be slower on CPU
    // friend constexpr QuaternionT<T> operator*(const QuaternionT<T>& a, const QuaternionT<T>& b) {
    //     const T scalar = a.s * b.s - Vector3T<T>::Dot(a.v, b.v);
    //     const Vector3T<T> imaginary = b.v * a.s + a.v * b.s + Vector3T<T>::Cross(a.v, b.v);
    //     return QuaternionT<T>(std::move(scalar), std::move(imaginary));
    // }

    friend constexpr QuaternionT<T> operator*(const QuaternionT<T>& a, const QuaternionT<T>& b) {
        return QuaternionT<T> (
            a.s*b.s - a.v.x*b.v.x - a.v.y*b.v.y - a.v.z*b.v.z,
            Vector3T<T> (
                a.s*b.v.x + a.v.x*b.s + a.v.y*b.v.z - a.v.z*b.v.y,
                a.s*b.v.y + a.v.y*b.s + a.v.z*b.v.x - a.v.x*b.v.z,
                a.s*b.v.z + a.v.z*b.s + a.v.x*b.v.y - a.v.y*b.v.x
            )
        );
    }

    constexpr QuaternionT<T> Inverse() const {
        return QuaternionT<T> (s, -v);
    }

    constexpr Vector3T<T> Rotate(Vector3T<T> point) const {
        const QuaternionT<T> q2 (this->s, -this->v);
        const QuaternionT<T> p (0, std::move(point));
        return QuaternionT<T>(*this * p * q2).v;
    }

#ifndef NO_MATRIX_DEP
    constexpr Matrix<4, 4, T> RotationMatrix() const {
        return Matrix<4, 4, T> {std::to_array<T>({
            1 - 2*v.y*v.y - 2*v.z*v.z, 2*v.x*v.y - 2*v.z*s,       2*v.x*v.z + 2*v.y*s,       0,
            2*v.x*v.y + 2*v.z*s,       1 - 2*v.x*v.x - 2*v.z*v.z, 2*v.y*v.z - 2*v.x*s,       0,
            2*v.x*v.z - 2*v.y*s,       2*v.y*v.z + 2*v.x*s,       1 - 2*v.x*v.x - 2*v.y*v.y, 0,
            0,                         0,                         0,                         1,
        })};
    }
#endif /* NO_MATRIX_DEP */

    friend std::ostream& operator<<(std::ostream& o, const QuaternionT<T> &q) {
        return o << q.s << ' ' << q.v;
    }
};

typedef QuaternionT<float> Quaternion;
