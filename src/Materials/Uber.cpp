#include "Uber.hpp"

Uber::Uber() : Material() {
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  Kr = new coeff(0, 0, 0);
  Kt = new coeff(0, 0, 0);
  spectreKd = spectreKs = spectreKr, spectreKt = nullptr;
  roughness = 0.1;
  uroughness = vroughness = 0;
  eta = 1.5;
  opacity = new coeff(1, 1, 1);
  spectreOp = nullptr;
  remaproughness = true;
}

Uber::Uber(const string &name) : Material(name) {
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  Kr = new coeff(0, 0, 0);
  Kt = new coeff(0, 0, 0);
  spectreKd = spectreKs = spectreKr, spectreKt = nullptr;
  roughness = 0.1;
  uroughness = vroughness = 0;
  eta = 1.5;
  opacity = new coeff(1, 1, 1);
  spectreOp = nullptr;
  remaproughness = true;
}


// un seul paramètre disponible de la forme :
// sprectrum texture  Kr
Uber::Uber(const string &name, const ParamSet &set)
  : Material(name){

  // initialisation des valeurs par défaut
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  Kr = new coeff(0, 0, 0);
  Kt = new coeff(0, 0, 0);
  spectreKd = spectreKs = spectreKr, spectreKt = nullptr;
  roughness = 0.1;
  uroughness = vroughness = 0;
  eta = 1.5;
  opacity = new coeff(1, 1, 1);
  spectreOp = nullptr;
  remaproughness = true;
  
  glmat.ks = *Ks;;
  glmat.kd = *Kd;
  glmat.shininess = 10/roughness;
  glmat.ka = glmat.kd;

  // recherche les valeurs de ks dans set
  vector <string> values;
  values = set.getValuesFor("Ks");
  if(values.size()!=0){
    string type = set.getTypeFor("Ks");
    if(type=="rgb" || type=="color"){
      glmat.ks.r = stof(values[0]);
      glmat.ks.g = stof(values[1]);
      glmat.ks.b = stof(values[2]);
      *Ks = glmat.ks;
    } else
      cout << "Uber : type " << type << "non géré" << endl;
  }
  
  // recherche les valeurs de kd dans set
  values.clear();
  values = set.getValuesFor("Kd");
  if(values.size()!=0){
    string type = set.getTypeFor("Kd");
    if(type=="rgb" || type=="color"){
      glmat.kd.r = stof(values[0]);
      glmat.kd.g = stof(values[1]);
      glmat.kd.b = stof(values[2]);
      *Kd = glmat.kd;
    } else
      cout << "Uber : type " << type << "non géré" << endl;
  }

  // recherche les valeurs de roughness dans set
  values.clear();
  values = set.getValuesFor("roughness");
  if(values.size()!=0){
    string type = set.getTypeFor("roughness");
    if(type=="float"){
      roughness = stof(values[0]);
      glmat.shininess = 10/roughness;
    }else
      cout << "Uber : type " << type << "non géré" << endl;
  }
  // on ne s'intéresse pas ici au reste
}



ostream& operator<<(ostream &out, const Uber &m){
  const Material *pm = &m;
  out << "materiau Uber " << *pm;

  return out;
}
