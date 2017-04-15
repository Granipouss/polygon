#include <windows.h>
#include <stdio.h>

#include "scene.h"
#include "system.h"
#include "graphics.h"

#define RBUTTON 1
#define LBUTTON 2
#define XBUTTON 3

#define SELECT_MODE 1
#define ADD_MODE    2
#define MOVE_MODE   3
#define FACE_MODE   4

int actionMode = SELECT_MODE;
int mouseX, mouseY;

bool holdShift = false;
bool holdCtrl = false;
bool pressingRight = false;

void initSystem () {
  initScene();
  initGraphics(WIDTH, HEIGHT);
}

void updateSystem () {
  if (pressingRight) moveCamera(mouseX - WIDTH / 2, mouseY - HEIGHT / 2);
  refreshGraphics(actionMode);
}

void updateMousePos (WPARAM wParam, LPARAM lParam) {
  holdShift = wParam  & MK_SHIFT;
  holdCtrl = wParam  & MK_CONTROL;
  mouseX = LOWORD(lParam);
  mouseY = HIWORD(lParam);
}

void selectModeProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Click
  if (uMsg == WM_LBUTTONUP) {
    updateMousePos(wParam, lParam);
    select(mouseX, mouseY);
  }
  // Keyboard
  if (uMsg == WM_KEYDOWN) {
    if (wParam == 'M') {
      savePos();
      actionMode = MOVE_MODE;
    }
    if (wParam == 'A') actionMode = ADD_MODE;
    if (wParam == 'F') {
      deselectAll();
      actionMode = FACE_MODE;
    }
    if (wParam == VK_DELETE) deleteSelected();
    if (wParam == VK_CLEAR) deleteSelected();
  }
}

void moveModeProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  movePoint(mouseX, mouseY, holdShift);
  // Click
  if (uMsg == WM_LBUTTONUP) {
    updateMousePos(wParam, lParam);
    actionMode = SELECT_MODE;
  }
  // Keyboard
  if (uMsg == WM_KEYDOWN) {
    if (wParam == VK_ESCAPE) {
      revertPos();
      actionMode = SELECT_MODE;
    }
  }
}

void addModeProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Click
  if (uMsg == WM_LBUTTONUP) {
    updateMousePos(wParam, lParam);
    addPoint(mouseX, mouseY);
    actionMode = MOVE_MODE;
  }
  // Keyboard
  if (uMsg == WM_KEYDOWN) {
    if (wParam == VK_ESCAPE) actionMode = SELECT_MODE;
  }
}

void faceModeProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Click
  if (uMsg == WM_LBUTTONUP) {
    updateMousePos(wParam, lParam);
    if (addFace(mouseX, mouseY)) {
      actionMode = SELECT_MODE;
    }
  }
  // Keyboard
  if (uMsg == WM_KEYDOWN) {
    if (wParam == VK_ESCAPE) actionMode = SELECT_MODE;
  }
}

LRESULT CALLBACK WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (actionMode) {
    case SELECT_MODE: selectModeProc(hwnd, uMsg, wParam, lParam); break;
    case MOVE_MODE: moveModeProc(hwnd, uMsg, wParam, lParam); break;
    case FACE_MODE: faceModeProc(hwnd, uMsg, wParam, lParam); break;
    case ADD_MODE: addModeProc(hwnd, uMsg, wParam, lParam); break;
  }
  switch (uMsg) {
    case WM_MOUSEMOVE:
      updateMousePos(wParam, lParam);
    break;
    case WM_CLOSE:
      PostQuitMessage(0);
    break;
    case WM_DESTROY:
      return 0;
    case WM_KEYDOWN:
      if (wParam == 'Q') PostQuitMessage(0);
    break;
    case WM_RBUTTONUP: pressingRight = false; break;
    case WM_RBUTTONDOWN: pressingRight = true; break;
    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}
