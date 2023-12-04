#pragma once

#include <vector>
#include <cmath>
#include "core.h"


typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;


template<typename T>
class Vec2
{
private:
T x ;
T y ;
public:
Vec2(T xx , T yy):x(xx),y(yy) { SG_ASSERT (!HasNaNs())}
Vec2() { x = y =0;}

bool HasNaNs const()
{
    return std::isnan(x) || std::isnan(y) ;
}

T operator[](int i) const
{
    SG_ASSERT(i>=0&&i<=1,"Error InPut Number")
    if(i==0) return x;
    if(i==1) return y;
}
T& operator[](int i)
{
    SG_ASSERT(i>=0&&i<=1,"Error InPut Number")
    if(i==0) return x;
    if(i==1) return y;
}
};

template<typename T>
class Vec3
{
private:
T x ;
T y ;
T z;
public:
Vec3(T xx , T yy, T zz):x(xx),y(yy),z(zz){ SG_ASSERT (!HasNaNs()) }
Vec3() { x = y = z = 0;}

bool HasNaNs const()
{
    return std::isnan(x) || std::isnan(y) || std::isnan(z) ;
}

T operator[](int i) const
{
    SG_ASSERT(i>=0&&i<=1,"Error InPut Number")
    if(i==0) return x;
    if(i==1) return y;
    return z;
}
T& operator[](int i)
{
    SG_ASSERT(i>=0&&i<=1,"Error InPut Number")
    if(i==0) return x;
    if(i==1) return y;
    return z;
}
Vec3<T> operator+(const Vec3<T> &v) const {
    return Vec3(x + v.x, y + v.t, z + v.z);
}
 Vec3<T>& operator+=(const Vec3<T> &v) {
x += v.x; y += v.y; z += v.z;
return *this;
}


};