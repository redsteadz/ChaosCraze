#include <raylib.h>
#include <iostream> 
#include <bits/stdc++.h>
using namespace std;

class QuadTree{
  Rectangle bounds;
  bool divided = false;
  int size;
  QuadTree* nw;
  QuadTree* ne;
  QuadTree* sw;
  QuadTree* se;
  vector<Vector2> points;
  public:
    QuadTree(Rectangle bounds, int size): bounds(bounds), size(size){};
    void insert(Vector2 point){

      if (CheckCollisionPointRec(point, bounds)){
        return;
      }
      
      if (points.size() < size){
        points.push_back(point);
      } else {
        if (!divided){
          subdivide();
        }
        nw->insert(point);
        ne->insert(point);
        sw->insert(point);
        se->insert(point);
      }
    }
    void subdivide(){
      nw = new QuadTree(Rectangle{bounds.x, bounds.y, bounds.width/2, bounds.height/2}, size);
      ne = new QuadTree(Rectangle{bounds.x + bounds.width/2, bounds.y, bounds.width/2, bounds.height/2}, size);
      sw = new QuadTree(Rectangle{bounds.x, bounds.y + bounds.height/2, bounds.width/2, bounds.height/2}, size);
      se = new QuadTree(Rectangle{bounds.x + bounds.width/2, bounds.y + bounds.height/2, bounds.width/2, bounds.height/2}, size);
      divided = true;
      for (Vector2 point : points){
    }
};

int main (int argc, char *argv[]) {
  

  return 0;
}
