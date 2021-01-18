#ifndef _VPBRT_SCENE_HPP
#define _VPBRT_SCENE_HPP

#include <vector>
#include <iostream>
using namespace std;

#include "Mesh.hpp"
#include "Objet.hpp"
#include "../rply-1.1.4/rply.h"
#include "transform.h"

	    
class Scene {
private:
  // objets nommés - non visibles
  vector <Objet *> objets;
  // les meshes visibles et leur ltm 
  vector <Transform *> ltm; // local transformation matrices
  vector <Mesh *> meshes;
  // statistiques sur la scène
  unsigned int nbFaces;
  unsigned int nbSommets;
  // bounding box de la scène
  BoundingBox bb;


public:
  Scene();
  ~Scene();

  
  void add(Mesh *m);
  void add(Mesh *m, Transform *t); 


  void add(Objet *o);
  
  void add(Scene *sc);
  void addInstance(const string &oname, const Transform &t);

  void draw();

  friend ostream& operator<<(ostream &out, const Scene &sc);
  
  
  bool load_ply(const char *filename, const Transform &ctm, Objet *cur);

  // int vertex_cb(p_ply_argument argument);
  // int face_cb(p_ply_argument argument);
  inline int getNbFaces(){ return nbFaces; }
  inline int getNbSommets(){ return nbSommets; }

  void printStats();

  BoundingBox getBB(){ return bb; }
  bool isInsideBB(const Point3f &p);

private:
  void updateBB(const BoundingBox &bbp);

};

#endif
