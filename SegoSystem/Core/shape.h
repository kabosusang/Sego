#pragma once
#include "Bounds.h"

class Shape{
private:
//const Transform *ObjectToWorld, *WorldToObject;
const bool reverseOrientation;
const bool transformSwapsHandedness;

public:
virtual Bounds3f ObjectBound() const = 0;
virtual Bounds3f WorldBound() const;


};

