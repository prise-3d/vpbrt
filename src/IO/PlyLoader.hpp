#ifndef _PLY_LOADER_HPP
#define _PLY_LOADER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#define PLY_ASCII 0
#define PLY_LITTLE_ENDIAN 1
#define PLY_BIG_ENDIAN 2


class PlyLoader {
private :
  vector <glm::vec3> points;
  vector <glm::vec3> normales;
  vector <glm::vec3> couleurs;
  vector <glm::ivec3> faces;
  vector <string> propNames;

  unsigned int nbSommets;
  unsigned int nbFaces;

  unsigned int format;
  string version;
  
  public:
  PlyLoader();
  ~PlyLoader();

  bool load(const string& name);

  void print();

private:
  bool readHeader(ifstream &in);
  bool readData(ifstream &in);
};

#endif
