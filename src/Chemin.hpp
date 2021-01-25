#ifndef _CHEMIN_HPP
#define _CHEMIN_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

struct Color {
    float r, g, b;
    Color() { r=g=b=0; }
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct Point {
  float x, y, z;
};

class Chemin {
public:
  int x, y; // pixel initial du chemin
  Color l; // luminance du chemin
  std::vector <Point> sommets; // sommets du chemin
  std::vector<Color> luminances; // luminances acquises

  Chemin(){};

  inline int size(){return sommets.size();}

  bool readPath(std::ifstream &in);
  void clear(){ sommets.clear(); }


  void draw(Color c);
  
  friend std::ostream& operator<<(std::ostream& out, const Chemin& path);

private:

  // set limit to change display of cylinder material
  float limit = 10000;

};

#endif
