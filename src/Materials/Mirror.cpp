#include "Mirror.hpp"

Mirror::Mirror() : Material() {
  Kr = new coeff(0.9, 0.9, 0.9);
  spectre = nullptr;    
}

Mirror::Mirror(const string &name) : Material(name) {
  Kr = new coeff(0.9, 0.9, 0.9);
  spectre = nullptr;
}


// un seul paramètre disponible de la forme :
// sprectrum texture  Kr
Mirror::Mirror(const string &name, const ParamSet &set)
  : Material(name){

  // initialisation des valeurs par défaut
  glmat.ks.r = glmat.ks.g = glmat.ks.b = 0.9;
  glmat.kd.r = glmat.kd.g = glmat.kd.b = 0.1;
  glmat.shininess = 100.0;
  glmat.ka = glmat.kd;
  Kr = new coeff(0.9, 0.9, 0.9);

  // recherche les valeurs de kr dans set
  vector <string> values;
  values = set.getValuesFor("Kr");
  if(values.size()==0) return;
  

  string type = set.getTypeFor("Kr");

  if(type=="rgb" || type=="color"){
      glmat.ks.r = stof(values[0]);
      glmat.ks.g = stof(values[1]);
      glmat.ks.b = stof(values[2]);
      *Kr = glmat.ks;
      return;
  }

  cout << "Mirror : type " << type << "non géré" << endl;

}



ostream& operator<<(ostream &out, const Mirror &m){
  const Material *pm = &m;
  out << "material mirror " << *pm;
  out << " " << m.glmat.ks.r << " " << m.glmat.ks.g << " " << m.glmat.ks.b << "   " << m.glmat.shininess << endl;
  return out;
}
