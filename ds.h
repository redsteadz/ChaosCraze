#ifndef QUADTREE_H
#define QUADTREE_H
#include <bits/stdc++.h>
#include <raylib.h>
template <typename T>
struct Point {
  Vector2 x;
  T* y;
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
  vector<Vector2> points;

public:
  QuadTree(Rectangle bounds, int size) : bounds(bounds), size(size){};

  bool insert(Vector2 point) {

    if (!CheckCollisionPointRec(point, bounds)) {
      return false;
    }

    if (points.size() < size) {
      points.push_back(point);
      return true;
    }
    // ELSE 
    if (!divided) {
      subdivide();
    }

    if (nw->insert(point)) {
      return true;
    } else if (ne->insert(point)) {
      return true;
    } else if (sw->insert(point)) {
      return true;
    } else if (se->insert(point)) {
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
    // for (Vector2 point : points) {
    //   nw->insert(point);
    //   ne->insert(point);
    //   sw->insert(point);
    //   se->insert(point);
    // }
  }
  void Draw() {
    DrawRectangleLinesEx(bounds, 1, RED);
    if (divided) {
      nw->Draw();
      ne->Draw();
      sw->Draw();
      se->Draw();
    }
  }


  vector<Vector2> query(Rectangle area){
    if (!CheckCollisionRecs(bounds, area)) {
      return {};
    }
    vector<Vector2> found;
    for(Vector2 point: points){
      if (CheckCollisionPointRec(point, area)) {
        found.push_back(point);
      }
    }

    if(divided){
      vector<Vector2> nw_found = nw->query(area);
      vector<Vector2> ne_found = ne->query(area);
      vector<Vector2> sw_found = sw->query(area);
      vector<Vector2> se_found = se->query(area);
      
      found.insert(found.end(), nw_found.begin(), nw_found.end());
      found.insert(found.end(), ne_found.begin(), ne_found.end());
      found.insert(found.end(), sw_found.begin(), sw_found.end());
      found.insert(found.end(), se_found.begin(), se_found.end());
    }
    return found;
  }
};
#endif
// int main(int argc, char *argv[]) { return 0; }
