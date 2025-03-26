#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <vector>

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { Add(object); }

    void Clear() { objects.clear(); }

    void Add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool Hit(const Ray& ray, Interval rayT, HitRecord& record) const override {
        HitRecord tempRecord;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto& object : objects) {
            if (object->Hit(ray, Interval(rayT.min, closestSoFar), tempRecord)) {
                hitAnything = true;
                closestSoFar = tempRecord.t;
                record = tempRecord;
            }
        }

        return hitAnything;
    }
};

#endif