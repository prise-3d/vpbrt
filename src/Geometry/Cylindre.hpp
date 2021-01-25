#ifndef _VPBRT_CYLINDRE_HPP
#define _VPBRT_CYLINDRE_HPP

#include <vector>
#include <iostream>
using namespace std;

#include "../Geometry/transform.h"
#include "../Materials/Material.hpp"
#include "BoundingBox.hpp"




class Cylindre {
private:
  float rayon, zmin, zmax; // attributs pbrt
  // attributs internes pour la visu
  Point3f deb, fin;
  //BoundingBox bb;
  Material *mat;

public:
  Cylindre();
  Cylindre(float r, float minz, float maxz);
  ~Cylindre();



  BoundingBox getBB();

  void draw();
  void draw(const Transform &t);

  void drawBB();
  void drawBB(const Transform &t);

  void transform(const Transform &t);

  void setMaterial(Material *m) { mat = m; }
  GLmaterial getGLmaterial();
  


  friend ostream& operator<<(ostream &out, const Cylindre &m);


};

#endif
