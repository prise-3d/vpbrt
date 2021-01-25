#ifndef _UBER_HPP
#define _UBER_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Uber : public Material {
private:
  // matériel défini par 3 coefficients de : réflexion diffuse,
  // réflexion glossy, réfexion speculaire, et transmission
  coeff *Kd, *Ks, *Kr, *Kt;
  // matériel défini par des spectres pour chacun des coeff précédents
  Spectrum *spectreKd, *spectreKs, *spectreKr, *spectreKt; 
  float roughness; // rugosité de la surface dans [0, 1]
  float uroughness, vroughness;
  float eta;
  coeff *opacity;
  Spectrum *spectreOp;
  bool remaproughness;
public:
  Uber();
  Uber(const string &name);
  Uber(const string &name, const ParamSet &set);


  friend ostream& operator<<(ostream &out, const Uber &m);


};

#endif
