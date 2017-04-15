#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "vec.h"
#include <gl/gl.h>
#include <gl/glu.h>

#define camSpeed 1e-5

class Camera {
  protected:
    double dist;
    double a, b;

  public:
    Camera (double _dist, double _a, double _b) : dist(_dist), a(_a), b(_b) {}

    void move (double db, double da) {
      a -= camSpeed * da;
      b -= camSpeed * db;
    }

    vec3 getPos () {
      return {
        dist * sin(a) * cos(b),
        dist * sin(a) * sin(b),
        dist * cos(a)
      };
    }

    void lookAt () {
      vec3 pos = getPos();
      gluLookAt(pos.x, pos.y, pos.z, 0, 0, 0, 0, 0, 1);
    }

    void lookAt (vec3 obj) {
      vec3 pos = getPos();
      gluLookAt(pos.x, pos.y, pos.z, obj.x, obj.y, obj.z, 0, 0, 1);
    }

  // virtual ~Camera ();
};

#endif // CAMERA_H_INCLUDED
