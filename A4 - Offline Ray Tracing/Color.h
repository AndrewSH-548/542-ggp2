#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <DirectXMath.h>

using Color = DirectX::XMFLOAT3;

void WriteColor(std::ostream& out, const Color& pixelColor) {
    auto r = pixelColor.x;
    auto g = pixelColor.y;
    auto b = pixelColor.z;

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
