#ifndef COLOR_H
#define COLOR_H

#include "Interval.h"
#include "Vector3.h"

using Color = Vector3;

inline double LinearToGamma(double linearComponent) {
    if (linearComponent > 0) 
        return std::sqrt(linearComponent);
    return 0;
}

void WriteColor(std::ostream& out, const Color& pixelColor) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Apply linearToGamma transform
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0, 0.999);
    int rbyte = int(256 * intensity.Clamp(r));
    int gbyte = int(256 * intensity.Clamp(g));
    int bbyte = int(256 * intensity.Clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
