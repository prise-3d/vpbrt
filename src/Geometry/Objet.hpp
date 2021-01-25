#ifndef _OBJET_HPP
#define _OBJET_HPP

#include <string>
#include <stack>
#include <vector>
using namespace std;

#include "transform.h"
#include "Mesh.hpp"
#include "Cylindre.hpp"

class Objet {
private :
  string name;
  vector <Mesh *> meshes;
  vector <Cylindre *> cylindres;
  // vector <Transform *> ltm; // local transformation matrix
public:
  Objet();
  Objet(const string &name);
  ~Objet();

  const string &getName();
  vector <Mesh*> getMeshes();
  vector <Cylindre*> getCylindres();

  void addMesh(Mesh *m);
  void addMesh(Mesh *m, const Transform &t);
  void addCylindre(Cylindre *m);
  void addCylindre(Cylindre *m, const Transform &t);

  BoundingBox getBB();
  void draw();

  friend ostream& operator<<(ostream &out, const Objet &obj);


};
#endif
