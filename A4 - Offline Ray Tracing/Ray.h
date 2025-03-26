#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& origin, const Vector3& direction) : origin(origin), direction(direction) {}

    const Point3& Origin() const { return origin; }
    const Vector3& Direction() const { return direction; }

    Point3 At(double magnitude) const {
        return origin + magnitude * direction;
    }

private:
    Point3 origin;
    Vector3 direction;
};

#endif