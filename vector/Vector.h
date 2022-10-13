#pragma once
#include <cmath>

#ifndef NO_MATRIX_DEP
#include "Matrix.h"
#endif /* NO_MATRIX_DEP */

#ifndef NO_MATRIX_DEP
template <size_t N = 3, typename T = float>
class VectorS : public Matrix<N, 1, T> {
    using real_t = typename std::conditional<
        std::is_floating_point<T>::value && (sizeof(T) >= sizeof(float)),
        T, float>::type;
public:
    constexpr VectorS() : Matrix<N, 1, T>() {}
    constexpr VectorS(const std::array<T, N>& data) : Matrix<N, 1, T>(data) {}
    constexpr VectorS(const VectorS<N-1, T>& other, T last) : Matrix<N, 1, T>() {
        size_t i;
        for (i = 0; i < other.n; i++) {
            this->data[i] = other.data[i];
        }
        this->data[i] = last;
    }
    constexpr VectorS(const VectorS<N, T>& other) : Matrix<N, 1, T>(other) {}
    template <typename _T>
    constexpr VectorS(const VectorS<N, _T>& other) : Matrix<N, 1, T>(other) {}
    constexpr VectorS(VectorS<N, T>&& other) : Matrix<N, 1, T>(std::move(other)) {}

    constexpr VectorS(const Matrix<N, 1, T>& other) : Matrix<N, 1, T>(other) {}
    template <typename _T>
    constexpr VectorS(const Matrix<N, 1, _T>& other) : Matrix<N, 1, T>(other) {}
    constexpr VectorS(Matrix<N, 1, T>&& other) : Matrix<N, 1, T>(std::move(other)) {}


    VectorS<N, T>& operator=(const VectorS<N, T>& other)& { this->data = other.data; return *this; }
    VectorS<N, T>& operator=(VectorS<N, T>&& other)& { this->data = std::move(other.data); return *this; }

    [[nodiscard]] explicit constexpr operator bool() const {
        for (const auto& e : *this) {
            if (e != 0) { return true; }
        }
        return false;
    }

    template <typename _T>
    [[nodiscard]] constexpr bool operator==(const VectorS<N, _T>& other) const {
        for (size_t i = 0; i < N; i++) {
            if (this->data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr real_t Magnitude() const {
        return std::sqrt(MagnitudeSqr());
    }

    [[nodiscard]] constexpr T MagnitudeSqr() const {
        T sum = 0;
        for (const auto& e : *this) {
            sum += e*e;
        }
        return sum;
    }

    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr VectorS<N, T> Normalized() const {
        return VectorS<N, T> { *this / Magnitude() };
    }

    ///Remember to check if magnitude is zero
    constexpr void Normalize() {
        *this /= Magnitude();
    }

    ///Remember to check if magnitude is zero
    constexpr void SetMagnitude(T mag) {
        *this *= mag / Magnitude();
    }

    ///Remember to check if magnitude is zero
    constexpr void ClampMagnitude(real_t mag) {
        if (Magnitude() > mag) { *this *= mag / Magnitude(); }
    }

    ///Remember to check if magnitude is zero
    constexpr void SetMax(T mag) {
        *this *= mag / Max();
    }

    [[nodiscard]] constexpr T Max() const {
        if (this->data.size() == 0) { return 0; }
        T tmax = this->data[0];
        for (const auto& e : *this) {
            if (e > tmax) {
                tmax = e;
            }
        }
        return tmax;
    }


    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr static real_t AngleBetween(const VectorS<N, T>& v1, const VectorS<N, T>& v2) {
        return std::acos(AngleBetweenCos(v1, v2));
    }

    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr static real_t AngleBetweenCos(const VectorS<N, T>& v1, const VectorS<N, T>& v2) {
        const real_t lenlen = v1.Magnitude() * v2.Magnitude();
        if (lenlen == 0) return 0;

        return Dot(v1, v2) / lenlen;
    }

    ///Can be negative.
    ///Remember to check if vProjectOn is zero
    [[nodiscard]] constexpr static real_t ProjectionLength(const VectorS<N, T>& v, const VectorS<N, T>& vProjectOn) {
        return Dot(v, vProjectOn) / vProjectOn.Magnitude();
    }

    [[nodiscard]] constexpr static VectorS<N, T> Projection(const VectorS<N, T>& v, const VectorS<N, T>& vProjectOn) {
        const real_t magsqr = vProjectOn.MagnitudeSqr();
        return vProjectOn * (Dot(v, vProjectOn) / magsqr);
    }

    [[nodiscard]] constexpr static VectorS<N, T> ProjectionOnPlane(const VectorS<N, T>& v, const VectorS<N, T>& planeNormal) {
        const real_t magsqr = planeNormal.MagnitudeSqr();
        return v - planeNormal * (Dot(planeNormal, v) / magsqr);
    }

    [[nodiscard]] constexpr static T Dot(const VectorS<N, T>& v1, const VectorS<N, T>& v2) {
        T sum = 0;
        for (size_t i = 0; i < N; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }

    [[nodiscard]] constexpr static VectorS<N, T> Lerp(VectorS<N, T> from, const VectorS<N, T>& to, float t) {
        for (size_t i = 0; i < N; i++) {
            from[i] += (to[i] - from[i]) * t;
        }
        return from;
    }

    friend std::ostream& operator<<(std::ostream& o, const VectorS<N, T> &v) {
        if (N == 0) { return o; }
        o << '{';
        for (size_t i = 0; i < N - 1; i++) {
            o << v[i] << ", ";
        }
        o << v[N - 1] << '}' << std::endl;
        return o;
    }
};
#endif /* NO_MATRIX_DEP */



template <typename T>
struct Vector3T {
    using real_t = typename std::conditional<
        std::is_floating_point<T>::value && (sizeof(T) >= sizeof(float)),
        T, float>::type;
    Vector3T() = default;
    constexpr Vector3T(T x, T y, T z) : x(x), y(y), z(z) { }
    explicit constexpr Vector3T(T v) : x(v), y(v), z(v) { }
    explicit constexpr Vector3T(const std::array<T, 3>& v) : x(v[0]), y(v[1]), z(v[2]) { }
    constexpr Vector3T(const Vector3T<T>& v) : x(v.x), y(v.y), z(v.z) { }
    Vector3T<T>& operator=(const Vector3T<T>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    [[nodiscard]] explicit constexpr operator bool() const {
        return x || y || z;
    }
    [[nodiscard]] constexpr bool operator==(const Vector3T<T>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    [[nodiscard]] constexpr bool operator!=(const Vector3T<T>& other) const {
        return !(*this == other);
    }
    [[nodiscard]] constexpr T& operator[](int i) {
        return ((T*) this)[i];
    }
    [[nodiscard]] constexpr T operator[](int i) const {
        return ((T*) this)[i];
    }
    constexpr Vector3T<T>& operator+=(const Vector3T<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    constexpr Vector3T<T>& operator-=(const Vector3T<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    constexpr Vector3T<T>& operator*=(T v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
    constexpr Vector3T<T>& operator/=(T v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
    [[nodiscard]] friend constexpr Vector3T<T> operator*(const Vector3T<T>& v, T s) {
        return Vector3T<T> {v.x * s, v.y * s, v.z * s};
    }
    [[nodiscard]] friend constexpr Vector3T<T> operator*(T s, const Vector3T<T>& v) {
        return Vector3T<T> {v.x * s, v.y * s, v.z * s};
    }
    [[nodiscard]] constexpr Vector3T<T> operator/(T v) const {
        return Vector3T<T> {x / v, y / v, z / v};
    }
    [[nodiscard]] constexpr Vector3T<T> operator+(const Vector3T<T>& v) const {
        return Vector3T {x + v.x, y + v.y, z + v.z};
    }
    [[nodiscard]] constexpr Vector3T<T> operator-(const Vector3T<T>& v) const {
        return Vector3T {x - v.x, y - v.y, z - v.z};
    }
    [[nodiscard]] constexpr Vector3T<T> operator-() const {
        return Vector3T {-x, -y, -z};
    }
    [[nodiscard]] constexpr real_t Magnitude() const {
        return std::hypot(x, y, z);
    }
    [[nodiscard]] constexpr T MagnitudeSqr() const {
        return (x * x + y * y + z * z);
    }
    [[nodiscard]] static constexpr Vector3T<T> Rotate(const Vector3T<T>& vPoint, Vector3T<T> vAxis, real_t angle) {
        vAxis.Normalize();
        const real_t half_ang = angle / 2;
        const real_t sin_half_ang = std::sin(half_ang);
        const real_t s1 = std::cos(half_ang);
        const Vector3T<T> v1 {sin_half_ang * vAxis.x, sin_half_ang * vAxis.y, sin_half_ang * vAxis.z};
        const Vector3T<T> v3 = -v1;
        const real_t s12 = -Vector3T<T>::Dot(v1, vPoint);
        const Vector3T<T> v12 = vPoint * s1 + Vector3T<T>::Cross(v1, vPoint);
        return v3 * s12 + v12 * s1 + Vector3T<T>::Cross(v12, v3);
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr Vector3T<T> Normalized() const {
        return *this / Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void Normalize() {
        *this /= Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void SetMagnitude(real_t mag) {
        *this *= mag / Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void ClampMagnitude(real_t mag) {
        if (Magnitude() > mag) { *this *= mag / Magnitude(); }
    }
    [[nodiscard]] constexpr T Max() const {
        return std::max({std::abs(x), std::abs(y), std::abs(z)});
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr Vector3T<T> WithMax(T max) const {
        return *this * (real_t) max / Max();
    }
    ///Remember to check if magnitude is zero
    constexpr void SetMax(T max) {
        *this *= (real_t) max / Max();
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] static constexpr real_t AngleBetween(const Vector3T<T>& v1, const Vector3T<T>& v2) {
        return std::acos(AngleBetweenCos(v1, v2));
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] static constexpr real_t AngleBetweenCos(const Vector3T<T>& v1, const Vector3T<T>& v2) {
        const real_t lenlen = std::sqrt(v1.MagnitudeSqr() * v2.MagnitudeSqr());
        return Dot(v1, v2) / lenlen;
    }
    ///Can be negative
    [[nodiscard]] static constexpr real_t ProjectionLength(const Vector3T<T>& v, const Vector3T<T>& vProjectOn) {
        return Dot(v, vProjectOn) / vProjectOn.Magnitude();
    }
    [[nodiscard]] static constexpr Vector3T<T> Projection(const Vector3T<T>& v, const Vector3T<T>& vProjectOn) {
        const real_t magsqr = vProjectOn.MagnitudeSqr();
        return vProjectOn * (Dot(v, vProjectOn) / magsqr);
    }
    ///Aka perpendicular projection
    [[nodiscard]] static constexpr Vector3T<T> ProjectionOnPlane(const Vector3T<T>& v, const Vector3T<T>& planeNormal) {
        const real_t magsqr = planeNormal.MagnitudeSqr();
        return v - planeNormal * (Dot(planeNormal, v) / magsqr);
    }
    [[nodiscard]] static constexpr T Dot(const Vector3T<T>& v1, const Vector3T<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    [[nodiscard]] static constexpr Vector3T<T> Cross(const Vector3T<T>& v1, const Vector3T<T>& v2) {
        return Vector3T<T> {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
    }
    [[nodiscard]] static constexpr Vector3T<T> Lerp(const Vector3T<T>& from, const Vector3T<T>& to, real_t t) {
        return Vector3T<T> {from.x + (to.x - from.x) * t, from.y + (to.y - from.y) * t, from.z + (to.z - from.z) * t};
    }
    [[nodiscard]] static constexpr real_t Distance(const Vector3T<T>& a, const Vector3T<T>& b) {
        return (b - a).Magnitude();
    }
    friend std::ostream& operator<<(std::ostream& o, const Vector3T<T> &v) {
        return o << '{' << v.x << ", " << v.y << ", " << v.z << '}';
    }
#ifndef NO_MATRIX_DEP
    [[nodiscard]] constexpr operator VectorS<3, T>() const { return VectorS<3, T>({x, y, z}); }
    [[nodiscard]] constexpr VectorS<4, T> Homogeneous(T w) const { return VectorS<4, T>({x, y, z, w}); }
    [[nodiscard]] constexpr Matrix<4, 4, T> TranslationMatrix() const {
        return Matrix<4, 4, T> {std::to_array<T>({
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1,
        })};
    }
    [[nodiscard]] constexpr Matrix<4, 4, T> ScaleMatrix() const {
        return Matrix<4, 4, T> {std::to_array<T>({
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1,
        })};
    }
#endif /* NO_MATRIX_DEP */
    T x, y, z;
};


template <typename T>
struct Vector2T {
    using real_t = typename std::conditional<
        std::is_floating_point<T>::value && (sizeof(T) >= sizeof(float)),
        T, float>::type;
    Vector2T() = default;
    constexpr Vector2T(T x, T y) : x(x), y(y) { }
    explicit constexpr Vector2T(T v) : x(v), y(v) { }
    explicit constexpr Vector2T(const std::array<T, 2>& v) : x(v[0]), y(v[1]) { }
    constexpr Vector2T(const Vector2T<T>& v) : x(v.x), y(v.y) { }
    Vector2T<T>& operator=(const Vector2T<T>& v) {
        x = v.x;
        y = v.y;
        return *this;
    }
    [[nodiscard]] explicit constexpr operator bool() const {
        return x || y;
    }
    [[nodiscard]] constexpr bool operator==(const Vector2T<T>& other) const {
        return x == other.x && y == other.y;
    }
    [[nodiscard]] constexpr bool operator!=(const Vector2T<T>& other) const {
        return !(*this == other);
    }
    [[nodiscard]] constexpr T& operator[](int i) {
        return ((T*) this)[i];
    }
    [[nodiscard]] constexpr T operator[](int i) const {
        return ((T*) this)[i];
    }
    constexpr Vector2T<T>& operator+=(const Vector2T<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    constexpr Vector2T<T>& operator-=(const Vector2T<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    constexpr Vector2T<T>& operator*=(T v) {
        x *= v;
        y *= v;
        return *this;
    }
    constexpr Vector2T<T>& operator/=(T v) {
        x /= v;
        y /= v;
        return *this;
    }
    [[nodiscard]] friend constexpr Vector3T<T> operator*(const Vector2T<T>& v, T s) {
        return Vector2T<T> {v.x * s, v.y * s};
    }
    [[nodiscard]] friend constexpr Vector3T<T> operator*(T s, const Vector2T<T>& v) {
        return Vector2T<T> {v.x * s, v.y * s};
    }
    [[nodiscard]] constexpr Vector2T<T> operator/(T v) const {
        return Vector2T<T> {x / v, y / v};
    }
    [[nodiscard]] constexpr Vector2T<T> operator+(const Vector2T<T>& v) const {
        return Vector2T {x + v.x, y + v.y};
    }
    [[nodiscard]] constexpr Vector2T<T> operator-(const Vector2T<T>& v) const {
        return Vector2T {x - v.x, y - v.y};
    }
    [[nodiscard]] constexpr Vector2T<T> operator-() const {
        return Vector2T {-x, -y};
    }
    [[nodiscard]] constexpr real_t Magnitude() const {
        return std::hypot(x, y);
    }
    [[nodiscard]] constexpr T MagnitudeSqr() const {
        return (x * x + y * y);
    }
    [[nodiscard]] static constexpr Vector2T<T> Rotate(const Vector2T<T>& vPoint, real_t angle) {
        real_t c = std::cos(angle);
        real_t s = std::sin(angle);
        return Vector2T<T> {vPoint.x * c - vPoint.y * s, vPoint.x * s + vPoint.y * c};
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr Vector2T<T> Normalized() const {
        return *this / Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void Normalize() {
        *this /= Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void SetMagnitude(real_t mag) {
        *this *= mag / Magnitude();
    }
    ///Remember to check if magnitude is zero
    constexpr void ClampMagnitude(real_t mag) {
        if (Magnitude() > mag) { *this *= mag / Magnitude(); }
    }
    [[nodiscard]] constexpr T Max() const {
        return std::max(std::abs(x), std::abs(y));
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] constexpr Vector2T<T> WithMax(T max) const {
        return *this * (real_t) max / Max();
    }
    ///Remember to check if magnitude is zero
    constexpr void SetMax(T max) {
        *this *= (real_t) max / Max();
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] static constexpr real_t AngleBetween(const Vector2T<T>& v1, const Vector2T<T>& v2) {
        return std::acos(AngleBetweenCos(v1, v2));
    }
    ///Remember to check if magnitude is zero
    [[nodiscard]] static constexpr real_t AngleBetweenCos(const Vector2T<T>& v1, const Vector2T<T>& v2) {
        const real_t lenlen = std::sqrt(v1.MagnitudeSqr() * v2.MagnitudeSqr());
        return Dot(v1, v2) / lenlen;
    }
    ///Can be negative
    [[nodiscard]] static constexpr real_t ProjectionLength(const Vector2T<T>& v, const Vector2T<T>& vProjectOn) {
        return Dot(v, vProjectOn) / vProjectOn.Magnitude();
    }
    [[nodiscard]] static constexpr Vector2T<T> Projection(const Vector2T<T>& v, const Vector2T<T>& vProjectOn) {
        const T magsqr = vProjectOn.MagnitudeSqr();
        return vProjectOn * (Dot(v, vProjectOn) / magsqr);
    }
    [[nodiscard]] static constexpr T Dot(const Vector2T<T>& v1, const Vector2T<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
    [[nodiscard]] static constexpr Vector2T<T> Lerp(const Vector2T<T>& from, const Vector2T<T>& to, real_t t) {
        return Vector2T<T> {from.x + (to.x - from.x)* t, from.y + (to.y - from.y)* t};
    }
    [[nodiscard]] static constexpr real_t Distance(const Vector2T<T>& a, const Vector2T<T>& b) {
        return (b - a).Magnitude();
    }
    friend std::ostream& operator<<(std::ostream& o, const Vector2T<T> &v) {
        return o << '{' << v.x << ", " << v.y << '}';
    }
#ifndef NO_MATRIX_DEP
    [[nodiscard]] constexpr operator VectorS<2, T>() { return VectorS<2, T>({x, y}); }
    [[nodiscard]] constexpr VectorS<3, T> Homogeneous(T w) { return VectorS<3, T>({x, y, w}); }
    [[nodiscard]] constexpr Matrix<3, 3, T> TranslationMatrix() const {
        return Matrix<3, 3, T> {std::to_array<T>({
            1, 0, x,
            0, 1, y,
            0, 0, 1,
        })};
    }
    [[nodiscard]] constexpr Matrix<3, 3, T> ScaleMatrix() const {
        return Matrix<3, 3, T> {std::to_array<T>({
            x, 0, 0,
            0, y, 0,
            0, 0, 1,
        })};
    }
#endif /* NO_MATRIX_DEP */
    T x, y;
};

typedef Vector3T<float> Vector3;
typedef Vector2T<float> Vector2;
