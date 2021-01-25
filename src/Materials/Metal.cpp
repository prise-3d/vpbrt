#include "Metal.hpp"

Metal::Metal() : Material() {
  roughness = 0.1;
  eta = nullptr;
  k = nullptr;
}

Metal::Metal(const string &name) : Material(name) {
  roughness = 0.1;
  eta = nullptr;
  k = nullptr;
}

Metal::Metal(const string &name, const ParamSet &set)
  : Material(name){
  // on ne traite pas les paramètres pour le moment

  // on affecte des valeurs aux composantes opengl en choisissant
  // le metal "chrome" dans la liste http://devernay.free.fr/cours/opengl/materials.html
  glmat.ka.r = 0.25;
  glmat.ka.g = 0.25;
  glmat.ka.b = 0.25;
  glmat.kd.r = 0.4;
  glmat.kd.g = 0.4;
  glmat.kd.b = 0.4;
  glmat.ks.r = 0.774597;
  glmat.ks.g = 0.774597;
  glmat.ks.b = 0.774597;
  glmat.shininess = 0.6 * 128;
  
}

ostream& operator<<(ostream &out, const Metal &m){
  const Material *pm = &m;
  out << "material metal " << *pm;
  return out;
}
