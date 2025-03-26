#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double DegreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double RandomDouble() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

// Common Headers

#include "Color.h"
#include "Ray.h"
#include "Vector3.h"
#include "Interval.h"

#endif