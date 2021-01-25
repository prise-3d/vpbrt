#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

// #include "ParamSet.hpp"

// enum MAT_VALUE {UNKNOWN, DISNEY, FOURIER, GLASS, HAIR, KDSUBURFACE, MATTE, METAL,
// 		  MIRROR, MIX, NONE, PLASTIC, SUBSTRATE, SUBSURFACE, TRANSLUCENT, UBER};

// http://devernay.free.fr/cours/opengl/materials.html

// définition des structures utiles pour représenter les matériaux opengl
struct coeff {
  float r, g, b, a;
  coeff(){ a = 1.0;}
  coeff(float r, float g, float b, float a=1.0) : r(r), g(g), b(b), a(a) {}
};

struct GLmaterial {
  coeff ka, kd, ks;
  float shininess;
  GLmaterial(){
    kd.r = kd.g = kd.b = 0.8;
    ka.r = kd.g = kd.b = 0.2;
    ks.r = ks.g = ks.b = 0.0;
    shininess = 0.0;
  }
    
};

struct Spectrum {
  vector <float> lambda;
  vector <float> value;
};


class Material {
protected:
  string name; // nom du materiel
  GLmaterial glmat; // paramètres du matériau opengl correspondant
public:
  Material();
  Material(const string &n);

  inline string getName() const { return name; }
  GLmaterial getGLmaterial(){ return glmat; }

  friend ostream& operator<<(ostream &out, const Material &m);

// private:
//   MAT_VALUE decodeType(const string &v);
//   string typeToString(MAT_VALUE v) const;


};

#endif
