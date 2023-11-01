#pragma once

#include <limits>

template <typename T>
class Bounds2{
public:

};

template <typename T>
class Bounds3{
private:
T pMin;
T pMax;
public:
Bounds3(){
    T minNum = std::numeric_limits<T>::lowest();
    T maxNum = std::numeric_limits<T>::max();

}


};

typedef Bounds2<float> Bounds2f;
typedef Bounds2<int> Bounds2i;
typedef Bounds3<float> Bounds3f;
typedef Bounds3<int> Bounds3i;














