#ifndef QUADTREE_H
#define QUADTREE_H
#include <bits/stdc++.h>
#include <raylib.h>

class NPC;

struct Circle {
  Vector2 center;
  float radius;
};

template <typename T> struct Point {
  Vector2 v;
  NPC *data;
};
using namespace std;

class QuadTree {
  Rectangle bounds;
  bool divided = false;
  int size;
  QuadTree *nw;
  QuadTree *ne;
  QuadTree *sw;
  QuadTree *se;
  vector<Point<NPC>> points;

public:
  QuadTree(Rectangle bounds, int size) : bounds(bounds), size(size){};

  bool insert(Point<NPC> point) {

    if (!CheckCollisionPointRec(point.v, bounds)) {
      return false;
    }
    // cout << "Points size: " << points.size() << endl;
    if (points.size() < size) {
      points.push_back(point);
      return true;
    }
    // ELSE
    if (!divided) {
      subdivide();
    }

    if (nw->insert(point)) {
      // cout << "Inserted in NW" << endl;
      return true;
    } else if (ne->insert(point)) {
      // cout << "Inserted in NE" << endl;
      return true;
    } else if (sw->insert(point)) {
      // cout << "Inserted in SW" << endl;
      return true;
    } else if (se->insert(point)) {
      // cout << "Inserted in SE" << endl;
      return true;
    }
    return false;
  }
  void subdivide() {
    nw = new QuadTree(
        Rectangle{bounds.x, bounds.y, bounds.width / 2, bounds.height / 2},
        size);
    ne = new QuadTree(Rectangle{bounds.x + bounds.width / 2, bounds.y,
                                bounds.width / 2, bounds.height / 2},
                      size);
    sw = new QuadTree(Rectangle{bounds.x, bounds.y + bounds.height / 2,
                                bounds.width / 2, bounds.height / 2},
                      size);
    se = new QuadTree(Rectangle{bounds.x + bounds.width / 2,
                                bounds.y + bounds.height / 2, bounds.width / 2,
                                bounds.height / 2},
                      size);
    divided = true;
  }
  void Draw() {
    DrawRectangleLinesEx(bounds, 1, RED);
    if (divided) {
      // cout << "Drawing subdivided" << endl;
      nw->Draw();
      ne->Draw();
      sw->Draw();
      se->Draw();
    }
  }

  vector<NPC *> query(Circle area) {
    cout << "Querying for: " << area.center.x << ", " << area.center.y << endl;
    cout << "Bounds: " << bounds.x << ", " << bounds.y << ", " << bounds.width
         << ", " << bounds.height << endl;
    if (!CheckCollisionPointRec(area.center, bounds)) {
      cout << "No Collision" << endl;
      return {};
    }
    vector<NPC *> found;
    for (Point point : points) {
      if (CheckCollisionPointCircle(point.v, area.center, area.radius * 2)) {
        found.push_back(point.data);
      }
    }

    if (divided) {
      vector<NPC *> nw_found = nw->query(area);
      vector<NPC *> ne_found = ne->query(area);
      vector<NPC *> sw_found = sw->query(area);
      vector<NPC *> se_found = se->query(area);

      found.insert(found.end(), nw_found.begin(), nw_found.end());
      found.insert(found.end(), ne_found.begin(), ne_found.end());
      found.insert(found.end(), sw_found.begin(), sw_found.end());
      found.insert(found.end(), se_found.begin(), se_found.end());
    }
    return found;
  }
  // ~QuadTree(){
  //   delete nw;
  //   delete ne;
  //   delete sw;
  //   delete se;
  // }
};
#endif
// int main(int argc, char *argv[]) { return 0; }
