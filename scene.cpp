#include <math.h>
#include <vector>
#include "inc/vec.h"
#include "inc/ray.h"
#include "inc/face.h"
#include "inc/vertex.h"
#include "inc/camera.h"

#include "graphics.h"

Camera* camera;
std::vector<Face> faces;
std::vector<Vertex> vertices;

Selectable* selected;
Vertex* beingFaced[3] = { NULL, NULL, NULL };

void initScene () {
  camera = new Camera(5, M_PI / 3, M_PI / 4);

  // vertices.push_back(Vertex(0, 1, 1));
  // vertices.push_back(Vertex(1, 0, 1));
  // vertices.push_back(Vertex(0, 0, 2));
  // vertices.push_back(Vertex(0,-1, 1));

  // faces.push_back(Face(&vertices[0], &vertices[1], &vertices[2]));
  // faces.push_back(Face(&vertices[3], &vertices[1], &vertices[2]));
}

// = Camera ===

void moveCamera (int dx, int dy) { camera->move(dx, dy); }

// = Select ===

Vertex* getClosestVertex (const Ray ray, double &shortest) {
  Vertex* closest = NULL;
  shortest = 1e8;
  for (unsigned i = 0; i < vertices.size(); i++) {
    double dist;
    if (vertices[i].intersect(ray, dist)) {
      if (dist < shortest) { shortest = dist; closest = &vertices[i]; }
    }
  }
  return closest;
}

Face* getClosestFace (const Ray ray, double &shortest) {
  Face* closest = NULL;
  shortest = 1e8;
  for (unsigned i = 0; i < faces.size(); i++) {
    double dist;
    if (faces[i].intersect(ray, dist)) {
      if (dist < shortest) { shortest = dist; closest = &faces[i]; }
    }
  }
  return closest;
}

Selectable* getClosest (const Ray ray) {
  double distA, distB;
  Vertex* A = getClosestVertex(ray, distA);
  Face* B = getClosestFace(ray, distB);
  if (!A) return B;
  if (!B) return A;
  if (distA < distB) return A;
  return B;
}

void deselectAll () {
  selected = NULL;
  for (unsigned i = 0; i < faces.size(); i++) faces[i].deselect();
  for (unsigned i = 0; i < vertices.size(); i++) vertices[i].deselect();
  beingFaced[0] = NULL; beingFaced[1] = NULL; beingFaced[2] = NULL;
}

void selectOne (Selectable* newlySelected) {
  deselectAll();
  newlySelected->select();
  selected = newlySelected;
}

void select (int mouseX, int mouseY) {
  Ray ray = getRay(mouseX, mouseY);
  Selectable* newlySelected = getClosest(ray);
  if (newlySelected) {
    selectOne(newlySelected);
    return;
  }
  deselectAll();
}

Vertex* getSelectedVertex () { return dynamic_cast<Vertex*> (selected); }
Face* getSelectedFace () { return dynamic_cast<Face*> (selected); }

// = Move Points ===

vec3 savedPos;
void savePos () {
  Vertex* selectedVertex = getSelectedVertex();
  if (selectedVertex) savedPos = selectedVertex->pos;
}
void revertPos () {
  Vertex* selectedVertex = getSelectedVertex();
  if (selectedVertex) selectedVertex->pos = savedPos;
}

void movePoint (int mouseX, int mouseY, bool holdShift) {
  Ray ray = getRay(mouseX, mouseY);
  Vertex* selectedVertex = getSelectedVertex();
  if (!selectedVertex) return;
  vec3 v = selectedVertex->pos;
  if (holdShift) {
    vec3 inter = ray.intersect({ v.x, v.y, 0 }, ray.dir - dot(ray.dir, vUp) * vUp);
    selectedVertex->pos.z = inter.z;
  } else {
    vec3 inter = ray.intersect({ 0, 0, v.z }, vUp);
    selectedVertex->pos.x = inter.x;
    selectedVertex->pos.y = inter.y;
  }
}

// = Add point ===

void addPoint (int mouseX, int mouseY) {
  vertices.push_back(Vertex(0, 0, 0));
  selectOne(&vertices.back());
}

// = Add face ===

bool addFace (int mouseX, int mouseY) {
  double d; Ray ray = getRay(mouseX, mouseY);
  Vertex* v = getClosestVertex(ray, d);
  // No vertex
  if (!v) return false;
  v->toggle();
  // Deselecting
  if (beingFaced[0] == v) { beingFaced[0] = NULL; return false; }
  if (beingFaced[1] == v) { beingFaced[1] = NULL; return false; }
  if (beingFaced[2] == v) { beingFaced[2] = NULL; return false; }
  // Add a Vertex
  beingFaced[!beingFaced[0] ? 0 : (!beingFaced[1] ? 1 : 2)] = v;
  // Is done yet ?
  if (!beingFaced[0]) return false;
  if (!beingFaced[1]) return false;
  if (!beingFaced[2]) return false;
  // Then add a face
  faces.push_back(Face(beingFaced[0], beingFaced[1], beingFaced[2]));
  deselectAll();
  return true;
}

// = Delete ===

void deleteSelected () {
  // vertex
  Vertex* selectedVertex = getSelectedVertex();
  if (selectedVertex) {
    for (unsigned i = 0; i < faces.size(); i++)
      if (faces[i].contains(selectedVertex)) faces.erase(faces.begin() + i);
  }
  for (unsigned i = 0; i < vertices.size(); i++)
    if (&vertices[i] == selected) vertices.erase(vertices.begin() + i);
  for (unsigned i = 0; i < faces.size(); i++)
    if (&faces[i] == selected) faces.erase(faces.begin() + i);
  deselectAll();
}
