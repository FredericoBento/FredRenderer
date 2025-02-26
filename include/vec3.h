#ifndef VEC3_H
#define VEC3_H

#include <iostream> 
#include <cmath> 

template <typename T>
class Vec3
{
    public:
        T e[3];

        Vec3() : e {0, 0, 0} {}
        Vec3(T e0, T e1, T e2) : e {e0, e1, e2} {}

        T x() const { return e[0]; }
        T y() const { return e[1]; }
        T z() const { return e[2]; }

        Vec3<T> operator-() const { return Vec3<T>(-e[0], -e[1], -e[2]); }
        T operator[](int i) const { return e[i]; }
        T& operator[](int i) { return e[i]; }

        Vec3& operator += (const Vec3& v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vec3& operator -= (const Vec3& v)
        {
            e[0] -= v.e[0];
            e[1] -= v.e[1];
            e[2] -= v.e[2];
            return *this;
        }

        Vec3& operator *= (const T t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        
        Vec3& operator /= (const T t)
        {
            return *this *= 1/t;
        }

        T length() const 
        {
            return std::sqrt(length_squared());
        }

        T length_squared() const
        {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec) {
        os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
        return os;

    }
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec3<T>& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline Vec3<T> operator+(const Vec3<T>& u, const Vec3<T>& v)
{
    return Vec3(u.e[0] + v.e[0], u.e[1] + v[1], u.e[2] + v.e[2]);
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T>& u, const Vec3<T>& v)
{
    return Vec3<T>(u.e[0] - v.e[0], u.e[1] - v[1], u.e[2] - v.e[2]);
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T>& u, const Vec3<T>& v)
{
    return Vec3<T>(u.e[0] * v.e[0], u.e[1] * v[1], u.e[2] * v.e[2]);
}

template <typename T>
inline Vec3<T> operator*(double t, const Vec3<T>& v)
{
    return Vec3<T>(t * v.e[0], t * v.e[1], t * v.e[2]);
}


template <typename T>
inline Vec3<T> operator*(const Vec3<T>& v,double t)
{
    return t * v;
}


template <typename T>
inline Vec3<T> operator/(const Vec3<T>& v, double t)
{
    return (1/t) * v;
}

template <typename T>
inline double dot(const Vec3<T>& u, const Vec3<T>& v)
{
    return u.e[0] * v.e[0] 
         + u.e[1] * v.e[1]   
         + u.e[2] * v.e[2];   
}

template <typename T>
inline Vec3<T> cross(const Vec3<T>& u, const Vec3<T>& v)
{
    return Vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],    
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template <typename T>
inline Vec3<T> unit_vector(const Vec3<T>& v)
{
    return v / v.length();
}
#endif
