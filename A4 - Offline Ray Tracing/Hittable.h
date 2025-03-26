#ifndef HITTABLE_H
#define HITTABLE_H

#include "RTWeekend.h"

class Material;

class HitRecord {
public:
    Point3 point;
    Vector3 normal;
    shared_ptr<Material> material;
    double t;
    bool frontFace;

    void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal) {
        // Sets the hit record normal vector.
        // NOTE: The parameter 'outwardNormal' is assumed to have unit length.

        frontFace = Dot(ray.Direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
};

#endif