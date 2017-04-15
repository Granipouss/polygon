#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

#include <gl/gl.h>

#include "vec.h"
#include "vertex.h"
#include "selectable.h"

class Face : public Selectable {
  private:
    Vertex* A;
    Vertex* B;
    Vertex* C;

  public:
    Face (Vertex* _A, Vertex* _B, Vertex* _C) {
      A = _A; B = _B; C = _C;
      selected = false;
    }

    bool intersect (Ray ray, double &dist) {
      vec3 a = A->pos, b = B->pos, c = C->pos;
      vec3 i = ray.intersect(a, normal());
      vec3 ac = c - a, ab = b - a, ai = i - a;
      double u = (ai.x * ac.y - ai.y * ac.x) / (ab.x * ac.y - ab.y * ac.x);
      double v = (ab.x * ai.y - ab.y * ai.x) / (ab.x * ac.y - ab.y * ac.x);
      if (u < 0) return false;
      if (v < 0) return false;
      return (u + v < 1);
    }

    bool contains (const Vertex* D) {
      if (A == D) return true;
      if (B == D) return true;
      if (C == D) return true;
      return false;
    }

    float shadingLevel (vec3 camDir) {
      vec3 n = normal();
      return dot(dot(camDir, n) > 0 ? -n : n, normalize({ 0, 0.2f, 1.0f }));
    }

    vec3 normal () {
      vec3 a = A->pos, b = B->pos, c = C->pos;
      return normalize(cross(a - b, a - c));
    }

    void draw (vec3 camDir) {
      glBegin(GL_TRIANGLES);
      // Color shading
      if (selected) {
        glColor4ub(210, 210, 0, 230);
      } else {
        int grey = 128 + 96 * shadingLevel(camDir);
        glColor4ub(grey, grey, grey, 230);
      }
      A->glDraw();
      B->glDraw();
      C->glDraw();
      glEnd();
    }
};

#endif // FACE_H_INCLUDED
