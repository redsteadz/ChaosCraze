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
  T *data;
};
using namespace std;

template <typename T> class QuadTree {
  Rectangle bounds;
  bool divided = false;
  int size;
  QuadTree *nw;
  QuadTree *ne;
  QuadTree *sw;
  QuadTree *se;
  vector<Point<T>> points;

public:
  QuadTree(Rectangle bounds, int size) : bounds(bounds), size(size){};

  bool insert(Point<T> point) {

    if (!CheckCollisionPointRec(point.v, bounds)) {
      return false;
    }
    // cout << "Points size: " << points.size() << endl;
    if (points.size() < size) {
      // cout << point.v.x << " " << point.v.y << endl;
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
    nw = new QuadTree<T>(
        Rectangle{bounds.x, bounds.y, bounds.width / 2, bounds.height / 2},
        size);
    ne = new QuadTree<T>(Rectangle{bounds.x + bounds.width / 2, bounds.y,
                                bounds.width / 2, bounds.height / 2},
                      size);
    sw = new QuadTree<T>(Rectangle{bounds.x, bounds.y + bounds.height / 2,
                                bounds.width / 2, bounds.height / 2},
                      size);
    se = new QuadTree<T>(Rectangle{bounds.x + bounds.width / 2,
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

  vector<Point<T>> query(Circle area) {
    // cout << "Querying for: " << area.center.x << ", " << area.center.y << endl;
    // cout << "Bounds: " << bounds.x << ", " << bounds.y << ", " << bounds.width
    //      << ", " << bounds.height << endl;
    if (!CheckCollisionPointRec(area.center, bounds)) {
      // cout << "No Collision" << endl;
      return {};
    }
    vector<Point<T>> found;
    for (Point<T> point : points) {
      if (CheckCollisionPointCircle(point.v, area.center, area.radius*2)) {
        // cout << "Query Data" << point.v.x << " " << point.v.y << endl;
        found.push_back(point);
      };
    };
    if (divided) {
      vector<Point<T>> nw_found = nw->query(area);
      vector<Point<T>> ne_found = ne->query(area);
      vector<Point<T>> sw_found = sw->query(area);
      vector<Point<T>> se_found = se->query(area);

      found.insert(found.end(), nw_found.begin(), nw_found.end());
      found.insert(found.end(), ne_found.begin(), ne_found.end());
      found.insert(found.end(), sw_found.begin(), sw_found.end());
      found.insert(found.end(), se_found.begin(), se_found.end());
      
    }
    return found;
  }

    vector<Point<T>> query(Rectangle area) {
    // cout << "Querying for: " << area.center.x << ", " << area.center.y << endl;
    // cout << "Bounds: " << bounds.x << ", " << bounds.y << ", " << bounds.width
    //      << ", " << bounds.height << endl;
    if (!CheckCollisionRecs(area , bounds)) {
      // cout << "No Collision" << endl;
      return {};
    }
    vector<Point<T>> found;
    for (Point point : points) {
      if (CheckCollisionPointRec(point.v, area)) {
        // cout << "Query Data" << point.v.x << " " << point.v.y << endl;
        found.push_back(point);
      }
    }

    if (divided) {
      vector<Point<T>> nw_found = nw->query(area);
      vector<Point<T>> ne_found = ne->query(area);
      vector<Point<T>> sw_found = sw->query(area);
      vector<Point<T>> se_found = se->query(area);

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
