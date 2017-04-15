#ifndef SELECTABLE_H_INCLUDED
#define SELECTABLE_H_INCLUDED

#include "ray.h"

class Selectable {
  public:
    bool selected;
    Selectable () : selected(false) {}

    void select () { selected = true; }
    void deselect () { selected = false; }
    bool toggle (bool value) { selected = value; }
    bool toggle () { selected = !selected; return selected; }

    virtual bool intersect (const Ray ray, double &dist) = 0;
};

#endif // SELECTABLE_H_INCLUDED
