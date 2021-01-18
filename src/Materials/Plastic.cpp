#include "Plastic.hpp"

Plastic::Plastic() : Material() {
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  spectreKd = spectreKs = nullptr;
  roughness = 0.1;
  remaproughness = true;
}

Plastic::Plastic(const string &name) : Material(name) {
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  spectreKd = spectreKs = nullptr;
  roughness = 0.1;
  remaproughness = true;
}


// un seul paramètre disponible de la forme :
// sprectrum texture  Kr
Plastic::Plastic(const string &name, const ParamSet &set)
  : Material(name){

  // initialisation des valeurs par défaut
  Kd = new coeff(0.25, 0.25, 0.25);
  Ks = new coeff(0.25, 0.25, 0.25);
  spectreKd = spectreKs = nullptr;
  roughness = 0.1;
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
      cout << "Plastic : type " << type << "non géré" << endl;
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
      cout << "Plastic : type " << type << "non géré" << endl;
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
      cout << "Plastic : type " << type << "non géré" << endl;
  }

}



ostream& operator<<(ostream &out, const Plastic &m){
  const Material *pm = &m;
  out << "materiau mirror " << *pm;

  return out;
}
