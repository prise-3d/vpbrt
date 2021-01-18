#ifndef _PBRT_LOADER_HPP
#define _PBRT_LOADER_HPP

#include <string>
#include <stack>
#include <vector>
using namespace std;

#include "../Geometry/Scene.hpp"
#include "../Geometry/transform.h"
#include "../Geometry/Objet.hpp"
#include "../Materials/Material.hpp"
#include "../Materials/ParamSet.hpp"

struct SysCoord {
  string name;
  Transform ctm;

  SysCoord(const string &n, const Transform &t) : name(n), ctm(t) {}
};

class PbrtLoader {
private :
  // voir core/transform.cpp - .hpp
  Transform ctm; // current transformation matrix
  stack <Transform> pileMatrices;
  // tampon interne pour les objets nommés
  Objet *curObj;
  // liste des systèmes de coordonnées nommés
  vector <SysCoord *> sysCoord;
  // liste des matériaux nommés
  vector <Material *> Materials;
  
public:
  PbrtLoader();
  ~PbrtLoader();

  Scene *load(const string &filename);

private:
  void loadShape(ifstream &in, const string& path, Scene *scene, Objet *cur);
  void loadPlymesh(ifstream &in, const string& path, Scene *scene, Objet *cur);
  void loadTriangleMesh(ifstream &in, const string& path, Scene *scene, Objet *cur);
  // void transformer(Vector4 &v);
  // void transformer(Point3f &s);
  bool readParameterList(ifstream & in, string &type, string &name, vector<string> &values);
  bool readList(ifstream & in, vector<string> &values);
  string getToken(ifstream &in);
  Material *createMaterial(const string &name, const ParamSet &set);


};
#endif
