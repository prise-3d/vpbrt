#ifndef _MIRROR_HPP
#define _MIRROR_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Mirror : public Material {
private:
  coeff *Kr; // matériel défini par 3 Kr
  Spectrum *spectre; // matériel défini par un Kr par longueur d'onde
public:
  Mirror();
  Mirror(const string &name);
  Mirror(const string &name, const ParamSet &set);

  friend ostream& operator<<(ostream &out, const Mirror &m);


};

#endif
