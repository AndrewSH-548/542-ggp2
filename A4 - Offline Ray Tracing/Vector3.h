#ifndef Vector3_H
#define Vector3_H

#include "RTWeekend.h"

class Vector3 {
public:
    double points[3];

    Vector3() : points{ 0,0,0 } {}
    Vector3(double x, double y, double z) : points{ x, y, z} {}

    double x() const { return points[0]; }
    double y() const { return points[1]; }
    double z() const { return points[2]; }

    Vector3 operator-() const { return Vector3(-points[0], -points[1], -points[2]); }
    double operator[](int index) const { return points[index]; }
    double& operator[](int index) { return points[index]; }

    Vector3& operator+=(const Vector3& vector) {
        points[0] += vector.points[0];
        points[1] += vector.points[1];
        points[2] += vector.points[2];
        return *this;
    }

    Vector3& operator*=(double scalar) {
        points[0] *= scalar;
        points[1] *= scalar;
        points[2] *= scalar;
        return *this;
    }

    Vector3& operator/=(double divisor) {
        return *this *= 1 / divisor;
    }

    double Length() const {
        return std::sqrt(LengthSquared());
    }

    double LengthSquared() const {
        return points[0] * points[0] + points[1] * points[1] + points[2] * points[2];
    }

    bool NearZero() const {
        double s = 1e-8;
        return fabs(points[0]) < s && fabs(points[1]) < s && fabs(points[2]) < s;
    }

    static Vector3 Random() {
        return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
    }

    static Vector3 Random(double min, double max) {
        return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }
};

// point3 is just an alias for Vector3, but useful for geometric clarity in the code.
using Point3 = Vector3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vector3& vector) {
    return out << vector.points[0] << ' ' << vector.points[1] << ' ' << vector.points[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return Vector3(u.points[0] + v.points[0], u.points[1] + v.points[1], u.points[2] + v.points[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return Vector3(u.points[0] - v.points[0], u.points[1] - v.points[1], u.points[2] - v.points[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return Vector3(u.points[0] * v.points[0], u.points[1] * v.points[1], u.points[2] * v.points[2]);
}

inline Vector3 operator*(double scalar, const Vector3& vector) {
    return Vector3(scalar * vector.points[0], scalar * vector.points[1], scalar * vector.points[2]);
}

inline Vector3 operator*(const Vector3& v, double t) {
    return t * v;
}

inline Vector3 operator/(const Vector3& v, double t) {
    return (1 / t) * v;
}

inline double Dot(const Vector3& u, const Vector3& v) {
    return u.points[0] * v.points[0]
        + u.points[1] * v.points[1]
        + u.points[2] * v.points[2];
}

inline Vector3 Cross(const Vector3& u, const Vector3& v) {
    return Vector3(u.points[1] * v.points[2] - u.points[2] * v.points[1],
        u.points[2] * v.points[0] - u.points[0] * v.points[2],
        u.points[0] * v.points[1] - u.points[1] * v.points[0]);
}

inline Vector3 UnitVector(const Vector3& vector) {
    return vector / vector.Length();
}

inline Vector3 RandomUnitVector() {
    while (true) {
        Vector3 p = Vector3::Random(-1, 1);
        double lensq = p.LengthSquared();
        if (1e-160 < lensq && lensq <= 1) return p / sqrt(lensq);
    }
}

inline Vector3 RandomInUnitDisk() {
    while (true) {
        Vector3 p = Vector3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if (p.LengthSquared() < 1) return p;
    }
}

inline Vector3 RandomOnHemisphere(const Vector3& normal) {
    Vector3 onUnitSphere = RandomUnitVector();
    return (Dot(onUnitSphere, normal) > 0) ? onUnitSphere : -onUnitSphere;
}

inline Vector3 Reflect(const Vector3& v1, const Vector3& v2) {
    return v1 - 2 * Dot(v1, v2) * v2;
}

inline Vector3 Refract(const Vector3& unitVector, const Vector3& normal, double etaiOverEtat) {
    double cosTheta = fmin(Dot(-unitVector, normal), 1);
    Vector3 rOutPerpendicular = etaiOverEtat * (unitVector + cosTheta * normal);
    Vector3 rOutParallel = -sqrt(fabs(1 - rOutPerpendicular.LengthSquared())) * normal;
    return rOutPerpendicular + rOutParallel;
}

#endif