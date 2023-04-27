#ifndef RTCAPSTONE_H
#define RTCAPSTONE_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "glm/glm.hpp"

using namespace std;
using namespace glm;
using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float clamp(float x, float min, float max) {
    return std::max(min, std::min(max, x));
}

inline float random_float() {
    // Returns a random real in [0,1).
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline float random_float(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_float();
}

inline static vec3 random() {
        return vec3(random_float(), random_float(), random_float());
}

inline static vec3 random(double min, double max) {
    return vec3(random_float(min,max), random_float(min,max), random_float(min,max));
}


#endif