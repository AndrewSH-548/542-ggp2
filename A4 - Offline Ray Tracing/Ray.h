#ifndef RAY_H
#define RAY_H

#include <DirectXMath.h>

using namespace DirectX;

class Ray {
public:
    Ray() {}

    Ray(const XMFLOAT3& origin, const XMVECTOR& direction) : origin(origin), direction(direction) {}

    const XMFLOAT3& Origin() const { return origin; }
    const XMVECTOR& Direction() const { return direction; }

    XMFLOAT3 At(double t) const {
        XMFLOAT3 result;
        XMStoreFloat3(&result, XMLoadFloat3(&origin) + t * direction);
        return result;
    }

private:
    XMFLOAT3 origin;
    XMVECTOR direction;
};

#endif