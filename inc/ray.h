#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include "vec.h"

struct Ray {
  vec3 origin, dir;
  vec3 intersect(vec3 pt, vec3 n) {
    double k = dot(n, pt - origin) / dot(n, dir);
    return origin + k * dir;
  }
};

#endif // RAY_H_INCLUDED
