#include "Glass.hpp"

Glass::Glass() : Material() {
  eta = 1.5;
  Kr = Kt = nullptr;
  uroughness = vroughness = 0;
  remaproughness = true;
}

Glass::Glass(const string &name) : Material(name) {
  eta = 1.5;
  Kr = Kt = nullptr;
  uroughness = vroughness = 0;
  remaproughness = true;
}

Glass::Glass(const string &name, const ParamSet &set)
  : Material(name){
  // on ne traite pas les paramètres pour le moment
  eta = 1.5;
  Kr = Kt = nullptr;
  uroughness = vroughness = 0;
  remaproughness = true;
  // on affecte des valeurs aux composantes opengl en choisissant
  // le metal "chrome" dans la liste http://devernay.free.fr/cours/opengl/materials.html
  glmat.ka.r = 0.1;
  glmat.ka.g = 0.1;
  glmat.ka.b = 0.1;
  glmat.ka.a = 0.1; // transparence
  glmat.kd.r = 0.1;
  glmat.kd.g = 0.1;
  glmat.kd.b = 0.1;
  glmat.kd.a = 0.1; // transparence
  glmat.ks.r = 0.774597;
  glmat.ks.g = 0.774597;
  glmat.ks.b = 0.774597;
  glmat.ks.a = 0.1; // transparence
  glmat.shininess = 0.6 * 128;
  
}

ostream& operator<<(ostream &out, const Glass &m){
  const Material *pm = &m;
  out << "materiau glass " << *pm;
  return out;
}
