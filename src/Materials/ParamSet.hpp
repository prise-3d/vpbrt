#ifndef _PARAMSET_HPP
#define _PARAMSET_HPP

#include <string>
#include <iostream>
#include <vector>
using namespace std;


class ParamSet {
private:
  vector <string> types, names;
  vector <vector <string>> values;
public:
  ParamSet();

  void addParameters(const string &type,
		     const string &name,
		     const vector <string> values);
  // string getType() const;
  vector <string> getValuesFor(const string &pname) const;
  string getTypeFor(const string &pname) const;

  friend ostream& operator<<(ostream &out, const ParamSet &mp);


};

#endif
