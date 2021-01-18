#ifndef _PLASTIC_HPP
#define _PLASTIC_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Plastic : public Material {
private:
  coeff *Kd, *Ks; // matériel défini par 3 Kd et 3 Ks
  Spectrum *spectreKd, *spectreKs; // matériel défini par un Kr par longueur d'onde
  float roughness; // rugosité de la surface dans [0, 1]
  bool remaproughness;
public:
  Plastic();
  Plastic(const string &name);
  Plastic(const string &name, const ParamSet &set);


  friend ostream& operator<<(ostream &out, const Plastic &m);


};

#endif
