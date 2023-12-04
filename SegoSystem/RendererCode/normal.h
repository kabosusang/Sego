#pragma once


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Normal3f
{
private:
float x,y,z;
public:
Normal3f():x(0),y(0),z(0) {}
explicit Normal3f(float xx,float yy,float zz):x(xx),y(yy),z(zz) {}

};

template<class T>
class Point
{
private:
T x,y,z;
public:
Point():x(0),y(0),z(0) {}
explicit Point(T xx,T yy,T zz):x(xx),y(yy),z(zz) {}

Vec3<T> operator+=(T t1) {return Vec3<T>(x+t1,y+t1,z+t1);}

};

typedef Point<float> Point3f;



template<class T>
class Vec3
{
private:
T x,y,z;
public:
Vec3():x(0),y(0),z(0) {}
Vec3(T xx,T yy,T zz):x(xx),y(yy),z(zz) {}

Vec3<T> operator*=(T t1) {return Vec3<T>(x*t1,y*t1,z*t1);}

};

typedef Vec3<float> Vector3f;


//Ray射线 r(t) = 0 + td 
class Ray
{
private:
Point3f o;
Vector3f d;
mutable float tMax; //记录最近相交处的偏移量
float time; //在有动画物体的场景中，渲染系统为每条射线构造处于合适时间的场景表示
//const Medium* medium; //端点的介质
public:
Ray():tMax(INFINITY),time(0.f){}
Ray(const Point3f &o1,const Vector3f &d1,float tm = INFINITY,
float time = 0.f):o(o1),d(d1),tMax(tm) {}

Point3f operator()(float t) const {return o+d*t;} 

};
Ray r(Point3f(0,0,0),Vector3f(1,2,3));

/*含有两条辅助射线的额外信息。这些额
外射线表示由胶卷平面上的主光线在 x 和 y 方向偏移一个样本距离后的相机
光线。通过确定三条射线在待着色物体上的投影区域，Texture可以为合适的
抗锯齿估计出要平均的面积。*/
class RayDifferential: public Ray{


};







