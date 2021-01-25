#ifndef _VPBRT_MESH_HPP
#define _VPBRT_MESH_HPP

#include <vector>
#include <iostream>
using namespace std;

#include "../Geometry/transform.h"
#include "../Materials/Material.hpp"
#include "BoundingBox.hpp"

// struct Sommet {
//   float x, y, z;
//   void print(){
//     cout << "(" << x << "," << y << "," << z << ")";
//   }
// };

struct Face {
  int isom[3];
};


	    
class Mesh {
private:
  vector <Point3f> sommets;
  vector <Face> faces;
  BoundingBox bb;
  Material *mat;

public:
  Mesh();
  ~Mesh();

  void add(const Point3f &s);
  void add(const Face &f);

  BoundingBox getBB();

  void draw();
  void draw(const Transform &t);

  void drawBB();
  void drawBB(const Transform &t);

  void setMaterial(Material *m) { mat = m; }
  GLmaterial getGLmaterial();
  

  inline int getNbFaces(){ return faces.size(); }
  inline int getNbSommets(){ return sommets.size(); }

  friend ostream& operator<<(ostream &out, const Mesh &m);

private:
  void updateBB(const Point3f &s);
};

#endif
