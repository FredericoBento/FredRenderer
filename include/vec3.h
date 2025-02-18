#ifndef VEC3_H
#define VEC3_H

#include <iostream> 

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

        Vec3& operator += (const Vec3& v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
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

        friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec) {
        os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
        return os;
    }
};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

#endif
