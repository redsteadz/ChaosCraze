#include "ds.h"
#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

const int height = 600;
const int width = 800;

void HandleClick(vector<Vector2> &points, QuadTree<Vector2> &q) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
      IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    int y = 1;
    points.push_back(mouse);
    Point<Vector2> p = {mouse, &points[points.size() - 1]};
    q.insert(p);
  } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) ||
             IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    Circle r = {mouse, 30};
    // DrawRectangleLinesEx(r, 2, BLUE);
    DrawCircleLines(mouse.x, mouse.y, r.radius, BLUE);
    vector<Vector2 *> c = q.query(r);
    cout << c.size() << endl;
    for (Vector2 *p : c) {
      DrawCircleV(*p, 4, WHITE);
    }
  }
}

int main(int argc, char *argv[]) {
  InitWindow(width, height, "QuadTree");
  SetExitKey(0);
  SetTargetFPS(60);
  Rectangle bounds = {0, 0, width, height};
  QuadTree<Vector2> Q(bounds, 4);
  vector<Vector2> points;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    HandleClick(points, Q);
    for (Vector2 p : points) {
      DrawCircleV(p, 2, GREEN);
    }

    Q.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
