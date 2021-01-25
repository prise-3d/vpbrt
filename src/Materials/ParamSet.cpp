#include "ParamSet.hpp"

ParamSet::ParamSet(){
}


void ParamSet::addParameters(const string &type,
			     const string &name,
			     const vector <string> values){
  types.push_back(type);
  names.push_back(name);
  this->values.push_back(values);
}

// string ParamSet::getType() const {
//   for(int i=0; i<names.size(); i++){
//     if(names[i]=="type") return values[i][0].substr(1, values[i][0].size()-2);
//   }
//   return "";
// }

vector <string> ParamSet::getValuesFor(const string &pname) const{
  for(int i=0; i<names.size(); i++){
    if(names[i]==pname) return values[i];
  }
  vector <string> vide;
  return vide;
}

string ParamSet::getTypeFor(const string &pname) const{
  for(int i=0; i<names.size(); i++){
    if(names[i]==pname) return types[i];
  }
  return "";
}

ostream& operator<<(ostream &out, const ParamSet &p){
  out << "ParamSet : " << endl;
  for(int i=0; i<p.types.size(); i++){
    out << p.types[i] << " " << p.names[i] << " : ";
    for(int v=0; v<p.values[i].size(); v++)
      out << p.values[i][v] << " ";
    out << endl;
  }
  return out;
}

