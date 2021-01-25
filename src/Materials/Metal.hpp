#ifndef _METAL_HPP
#define _METAL_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Metal : public Material {
private:
  float roughness; //[0, 1]
  Spectrum *eta; 
  Spectrum *k;
public:
  Metal();
  Metal(const string &name);
  Metal(const string &name, const ParamSet &set);


  friend ostream& operator<<(ostream &out, const Metal &m);


};

#endif
