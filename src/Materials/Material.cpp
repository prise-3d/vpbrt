#include "Material.hpp"

Material::Material(){
  name = "default";
}

Material::Material(const string &n){
  name = n;
  // string stype = s.getType();
  // cout << "Materiel " << name << " de type " << stype << endl;
  // cout << s << endl;
}



// MAT_VALUE Material::decodeType(const string &t){
//   string v = t.substr(1, t.size()-2);
//   if(v=="disney") return DISNEY;
//   if(v=="fourier") return FOURIER;
//   if(v=="glass") return GLASS;
//   if(v=="hair") return HAIR;
//   if(v=="kdsubsurface") return KDSUBURFACE;
//   if(v=="matte") return MATTE;
//   if(v=="metal") return METAL;
//   if(v=="mirror") return MIRROR;
//   if(v=="mix") return MIX;
//   if(v=="none") return NONE;
//   if(v=="plastic") return PLASTIC;
//   if(v=="substrate") return SUBSTRATE;
//   if(v=="subsurface") return SUBSURFACE;
//   if(v=="translucent") return TRANSLUCENT;
//   if(v=="uber") return UBER;
//   return UNKNOWN;
// }

// string Material::typeToString(MAT_VALUE v) const{
//   string names[]={"unknown", "disney", "fourier", "glass", "hair", "kdsubsurface", "matte", "metal", "mirror", "mix", "none", "plastic", "substrate", "subsurface", "translucent", "uber"};

//   return names[v];
// }

ostream& operator<<(ostream &out, const Material &m){
  out << "Matériel nommé " << m.name << endl;
  return out;
}
