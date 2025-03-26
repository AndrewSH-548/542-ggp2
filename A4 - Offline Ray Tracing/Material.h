#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool Scatter (const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        Vector3 scatterDirection = record.normal + RandomUnitVector();
        if (scatterDirection.NearZero()) scatterDirection = record.normal;

        scattered = Ray(record.point, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool Scatter (const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        Vector3 reflected = Reflect(rayIn.Direction(), record.normal);
        reflected = UnitVector(reflected) + fuzz * RandomUnitVector();
        scattered = Ray(record.point, reflected);
        attenuation = albedo;
        return Dot(scattered.Direction(), record.normal) > 0;
    }

private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1, 1, 1);
        double localRI = record.frontFace ? 1 / refractionIndex : refractionIndex;

        Vector3 unitDirection = UnitVector(rayIn.Direction());
        double cosTheta = fmin(Dot(-unitDirection, record.normal), 1);
        double sinTheta = sqrt(1 - cosTheta * cosTheta);
        
        bool cannotRefract = localRI * sinTheta > 1;
        Vector3 direction = cannotRefract || Reflectance(cosTheta, localRI) > RandomDouble() ?
            Reflect(unitDirection, record.normal) :
            Refract(unitDirection, record.normal, localRI);

        scattered = Ray(record.point, direction);
        return true;
    }

private:
    double refractionIndex;

    static double Reflectance(double cosine, double refractionIndex) {
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 *= r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif