#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> material) : center(center), radius(std::fmax(0, radius)), material(material) {}

    bool Hit(const Ray& ray, Interval rayT, HitRecord& record) const override {
        Vector3 originToCenter = center - ray.Origin();
        double a = ray.Direction().LengthSquared();
        double h = Dot(ray.Direction(), originToCenter);
        double c = originToCenter.LengthSquared() - radius * radius;

        double discriminant = h * h - a * c;
        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (h - sqrtd) / a;
        if (!rayT.Surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!rayT.Surrounds(root))
                return false;
        }

        record.t = root;
        record.point = ray.At(record.t);
        Vector3 outwardNormal = (record.point - center) / radius;
        record.SetFaceNormal(ray, outwardNormal);
        record.material = material;

        return true;
    }

private:
    Point3 center;
    double radius;
    shared_ptr<Material> material;
};

#endif