#include "Matte.hpp"

Matte::Matte() : Material() {
  Kd = nullptr;
  spectre = nullptr;
  sigma = 0;
}

Matte::Matte(const string &name) : Material(name) {
  Kd = nullptr;
  spectre = nullptr;
  sigma = 0;
}

// pour définir Kd, il semble qu'on puisse trouver :
// - rgb Kd, color Kd, xyz Kd, spectrum Kd
// - texture Kd
Matte::Matte(const string &name, const ParamSet &set)
  : Material(name){
 
  // recherche les valeurs de kd dans set
  vector <string> couleurs;
  couleurs = set.getValuesFor("Kd");
  if(couleurs.size()==0) return;
  string type = set.getTypeFor("Kd");

  if(type=="rgb" || type=="color"){
    // définition du materiel opengl
    glmat.kd.r = stof(couleurs[0]);
    glmat.kd.g = stof(couleurs[1]);
    glmat.kd.b = stof(couleurs[2]);
    glmat.ka.r = glmat.kd.r/4;
    glmat.ka.g = glmat.kd.g/4;
    glmat.ka.b = glmat.kd.b/4;
    // stockage du matériel interne
    Kd = new coeff();
    *Kd = glmat.kd;
    return;
  }
  
  if(type=="spectrum"){// non géré
    // les formats possibles sont :
    // "spectrum Kd" "filename"
    // "spectrum Kd" [  lambda1 v1 lambda2 v2 ... ]
    cout << "spectrum non géré" << endl;
  }
}



ostream& operator<<(ostream &out, const Matte &m){
  const Material *pm = &m;
  out << "material matte " << *pm;
  return out;
}
