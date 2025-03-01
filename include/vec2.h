#ifndef VEC2_H
#define VEC2_H

#include <iostream> 

template <typename T>
class Vec2
{
    public:
      T e[2];

      Vec2() : e {0, 0} {}
      Vec2(T e0, T e1) : e {e0, e1} {}

      T x() const { return e[0]; }
      T y() const { return e[1]; }

      T operator[](int i) const { return e[i]; }
      T& operator[](int i) { return e[i]; }

      Vec2<T> operator-() const { return Vec2<T>(-e[0], -e[1]); }

      Vec2& operator += (const Vec2& v)
      {
          e[0] += v.e[0];
          e[1] += v.e[1];
          return *this;
      }

      Vec2& operator *= (const T t)
      {
          e[0] *= t;
          e[1] *= t;
          return *this;
      }

      Vec2& operator /= (const T t)
      {
          return *this *= 1/t;
      }

      friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec) {
          os << "(" << vec.x() << ", " << vec.y() << ")";
          return os;
      }
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

template <typename T>
inline Vec2<T> operator+(const Vec2<T>& u, const Vec2<T>& v)
{
    return Vec2(u.e[0] + v.e[0], u.e[1] + v[1]);
}

template <typename T>
inline Vec2<T> operator-(const Vec2<T>& u, const Vec2<T>& v)
{
    return Vec2<T>(u.e[0] - v.e[0], u.e[1] - v[1]);
}

template <typename T>
inline Vec2<T> operator*(const Vec2<T>& u, const Vec2<T>& v)
{
    return Vec2<T>(u.e[0] * v.e[0], u.e[1] * v[1]);
}
template <typename T>
inline Vec2<T> operator*(double t, const Vec2<T>& v)
{
    return Vec2<T>(t * v.e[0], t * v.e[1]);
}

template <typename T>
inline Vec2<T> operator*(const Vec2<T>& v, double t)
{
    return t * v;
}
#endif 
