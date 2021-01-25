#ifndef _GLASS_HPP
#define _GLASS_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Material.hpp"
#include "ParamSet.hpp"

		  

class Glass : public Material {
private:
  float eta; //index de refraction
  Spectrum *Kr; // reflectivité de la surface
  Spectrum *Kt; // transmitivité de la surface
  float uroughness;
  float vroughness;
  bool remaproughness;
public:
  Glass();
  Glass(const string &name);
  Glass(const string &name, const ParamSet &set);


  friend ostream& operator<<(ostream &out, const Glass &m);


};

#endif
