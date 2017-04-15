#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "inc/ray.h"

void initGraphics (int height, int width);
void refreshGraphics (int mode);

Ray getRay (int mouseX, int mouseY);

#endif // GRAPHICS_H_INCLUDED
