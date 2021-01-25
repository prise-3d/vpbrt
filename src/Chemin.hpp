#ifndef _CHEMIN_HPP
#define _CHEMIN_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

struct Point {
  float x, y, z;
};

class Chemin {
public:
  int x, y; // pixel initial du chemin
  float r, g, b; // luminance du chemin
  std::vector <Point> sommets; // sommets du chemin

  Chemin(){};

  inline int size(){return sommets.size();}

  bool readPath(std::ifstream &in);
  void clear(){ sommets.clear(); }


  void draw(float r, float g, float b);
  
  friend std::ostream& operator<<(std::ostream& out, const Chemin& path);

};

#endif
