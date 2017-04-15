#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <vector>
#include "inc/vec.h"
#include "inc/face.h"
#include "inc/vertex.h"
#include "inc/camera.h"
#include "inc/selectable.h"

extern Camera* camera;
extern Selectable* selected;
extern std::vector<Face> faces;
extern std::vector<Vertex> vertices;

void initScene ();

void moveCamera (int mouseX, int mouseY);

void deselectAll ();
void select (int mouseX, int mouseY);
Vertex* getSelectedVertex();
void savePos (); void revertPos ();
void movePoint (int mouseX, int mouseY, bool holdShift);
void addPoint (int mouseX, int mouseY);
bool addFace (int mouseX, int mouseY);
void deleteSelected ();

#endif // SCENE_H_INCLUDED
