#ifndef _MATTE_HPP
#define _MATTE_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Matte : public Material {
private:
  coeff *Kd; // matériel défini par 3 Kd
  Spectrum *spectre; // matériel défini par un Kd par longueur d'onde
  float sigma;
public:
  Matte();
  Matte(const string &name);
  Matte(const string &name, const ParamSet &set);


  friend ostream& operator<<(ostream &out, const Matte &m);


};

#endif
