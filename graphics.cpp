#include <gl/gl.h>
#include <gl/glu.h>
#include "lib/loader/TGALoader.h"

#include <math.h>
#include <stdio.h>
#include "inc/vec.h"

#include "scene.h"

void drawFloorGrid ();
void drawOrthoLines ();
void drawPointLines (vec3 pos);

int screenHeight, screenWidth;

GLuint textureGuide;

void loadTexture (const char* filename, GLuint &texName) {
  NS_TGALOADER::IMAGE img;
  img.LoadTGA(filename);
  glGenTextures(1, &texName);
  glBindTexture(GL_TEXTURE_2D, texName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getDataForOpenGL());
}

void initGraphics (int width, int height) {
  screenHeight = height;
  screenWidth = width;
  glEnable(GL_LINE_STIPPLE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
  loadTexture ("img/guide.tga", textureGuide);
}

int graphDim;
void switchTo2D () {
  if (graphDim != 2) {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, 0.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    graphDim = 2;
  }
}

void switchTo3D () {
  if (graphDim != 3) {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (float) screenWidth / screenHeight, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    graphDim = 3;
  }
}

void render () {
  camera->lookAt();
  vec3 camDir = make_vec3(0) - camera->getPos();

  drawOrthoLines();
  drawFloorGrid();

  Vertex* selectedVertex = getSelectedVertex();
  if (selectedVertex) drawPointLines(selectedVertex->pos);

  for (unsigned i = 0; i < faces.size(); i++)
    faces[i].draw(camDir);

  for (unsigned i = 0; i < vertices.size(); i++)
    vertices[i].draw();
}

void drawImage (int x, int y, int w, int h, int dx, int dy, float tSize, GLuint texName) {
  glColor3ub(255, 255, 255);
  glBindTexture(GL_TEXTURE_2D, texName);
  glBegin(GL_QUADS);
  glTexCoord2d(dx / tSize, dy / tSize);  glVertex2d(x, y);
  glTexCoord2d(dx / tSize, (dy + h) / tSize);  glVertex2d(x,     y + h);
  glTexCoord2d((dx + w) / tSize, (dy + h) / tSize);  glVertex2d(x + w, y + h);
  glTexCoord2d((dx + w) / tSize, dy / tSize);  glVertex2d(x + w, y    );
  glEnd();
  glBindTexture(GL_TEXTURE_2D, NULL);
}

void renderGUI (int mode) {
  drawImage(10, 10, 192, 112, 0, 0, 256 + 4, textureGuide);
  drawImage(
    screenWidth  - 64 - 10,
    screenHeight - 32 - 10,
    64, 32, 64 * (mode - 1), 256 - 32,
    256 + 2, textureGuide
  );
}

void refreshGraphics (int mode) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  switchTo2D(); renderGUI(mode);
  switchTo3D(); render();
  glFlush();
}

void drawFloorGrid () {
  glLineWidth(1);
  glLineStipple(1, 0xAAAA);
  for (int i = -5; i <= 5; i++) {
    glColor4ub(255, 255, 255, i ? 32 : 64);
    glBegin(GL_LINES);
    glVertex3f(i,-5, 0); glVertex3f( i, 5, 0);
    glVertex3f(5, i, 0); glVertex3f(-5, i, 0);
    glEnd();
  }
  glLineStipple(1, 0xFFFF);
}

void drawOrthoLines () {
  glLineWidth(2.5f);
  glBegin(GL_LINES);
  glColor3ub(255, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
  glColor3ub(0, 255, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
  glColor3ub(0, 0, 255); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
  glEnd();
}

void drawPointLines (vec3 pos) {
  glLineWidth(1);
  glLineStipple(1, 0xBBBB);
  glBegin(GL_LINES);
  glColor3ub(255, 0, 0); glVertex3f(pos.x, pos.y, 0); glVertex3f(pos.x, 0, 0);
  glColor3ub(0, 255, 0); glVertex3f(pos.x, pos.y, 0); glVertex3f(0, pos.y, 0);
  glColor3ub(0, 0, 255); glVertex3f(pos.x, pos.y, 0); glVertex3f(pos.x, pos.y, pos.z);
  glEnd();
  glLineStipple(1, 0xFFFF);
}

Ray getRay (int mouseX, int mouseY) {
  switchTo3D();
  double matModelView[16], matProjection[16];
  int viewport[4];
  glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
  glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
  glGetIntegerv(GL_VIEWPORT, viewport);
  double winX = (double) mouseX;
  double winY = viewport[3] - (double) mouseY;
  vec3 p_start, p_end;
  gluUnProject(winX, winY, 0.0, matModelView, matProjection, viewport, &p_start.x, &p_start.y, &p_start.z);
  gluUnProject(winX, winY, 1.0, matModelView, matProjection, viewport, &p_end.x, &p_end.y, &p_end.z);
  return { p_start, normalize(p_end - p_start) };
}
