#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#include <stdio.h>
#include <math.h>

// = Definition ===

struct vec3 {
  double x, y, z;
  void print () { printf("(%f, %f, %f)\n", x, y, z); }
  double length2 () const { return x * x + y * y + z * z; }
  double length () const { return sqrt(length2()); }
};

// = Constructors ===

inline vec3 make_vec3 (double _x) {
  return { _x, _x, _x };
}

inline vec3 make_vec3 (double _x, double _y, double _z) {
  return { _x, _y, _z };
}

inline vec3 make_vec3 (vec3 v) {
  return { v.x, v.y, v.z };
}

// = Algebra ===

inline vec3 operator - (vec3 &a) { return { -a.x, -a.y, -a.z }; }
inline vec3 operator + (vec3 a, vec3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
inline vec3 operator - (vec3 a, vec3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
inline vec3 operator * (vec3 a, double b) { return { a.x * b, a.y * b, a.z * b }; }
inline vec3 operator * (double b, vec3 a) { return { b * a.x, b * a.y, b * a.z }; }
inline void operator += (vec3 &a, vec3 b) { a.x += b.x; a.y += b.y; a.z += b.z; }
inline void operator *= (vec3 &a, double b) { a.x *= b; a.y *= b; a.z *= b; }
// inline vec3 operator * (vec3 a, vec3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
// inline void operator *= (vec3 &a, vec3 b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; }

inline double dot(const vec3 a, const vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3 a, const vec3 b) {
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline vec3 normalize (const vec3 v) {
  double nor2 = v.length2();
  if (nor2 == 0) return make_vec3(0);
  return v * (1 / sqrt(nor2));
}

const vec3 vLeft     = make_vec3( 1, 0, 0);
const vec3 vRight    = make_vec3(-1, 0, 0);
const vec3 vForward  = make_vec3( 0, 1, 0);
const vec3 vBackward = make_vec3( 0,-1, 0);
const vec3 vUp       = make_vec3( 0, 0, 1);
const vec3 vDown     = make_vec3( 0, 0,-1);

#endif // VEC_H_INCLUDED
