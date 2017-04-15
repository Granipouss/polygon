#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <gl/gl.h>

#include "vec.h"
#include "selectable.h"

class Vertex : public Selectable {
public:
  vec3 pos;

  Vertex (vec3 _pos) : pos(_pos), Selectable() {}
  Vertex (float _x) : pos(make_vec3(_x)), Selectable() {}
  Vertex (float _x, float _y, float _z) : pos(make_vec3(_x, _y, _z)), Selectable() {}

  bool intersect (const Ray ray, double &dist) {
    vec3 OA = pos - ray.origin;
    vec3 HC = OA - dot(OA, ray.dir) * ray.dir;
    if (HC.length2() < 1e-2) {
      dist = OA.length2();
      return true;
    }
    return false;
  }

  void glDraw () { glVertex3d(pos.x, pos.y, pos.z); }

  void draw () {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    if (selected) {
      glColor3ub(255, 150, 40);
    } else {
      glColor3ub(255, 255, 255);
    }
    glDraw();
    glEnd();
  }
};

#endif // VERTEX_H_INCLUDED
