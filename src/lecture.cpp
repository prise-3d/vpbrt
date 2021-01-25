#include <string>
#include <iostream>
#include <fstream>
using namespace std;


// lecture d'un mot du flux d'entrée, en passant les lignes
// commençant par un # (commentaire)
string getToken(ifstream &in){
  string str;
  in >> str;
  while(!in.eof()){
    // cout << "token = [" << str << "]" << endl; 
    if(str[0]=='#'){
      getline(in, str);
      in >> str;
    }
    else return str;
  }
  
}
int lire(const string &path, const string &filename){
  string name = path+"/"+filename;
  cout << "name = " << path+"/"+filename;
  ifstream in(path+"/"+filename);
  if(!in.is_open()) return -1;

  string str;
  str = getToken(in);

  while(!in.eof()){
    cout << "[" << str << "]" << endl;
    if(str=="Include") {
      cout << "-------- INCLUDE -------------" << endl;
      str = getToken(in);
      cout << "res = " << lire(path, str.substr(1, str.length()-2)) << endl;;
    }
    str = getToken(in);
  }

  in.close();
  return 0;
}

int main(int argc, char *argv[]){
  // string filename(argv[1]);

  //   // extraction du chemin d'accès
  // size_t end_path = filename.find_last_of('/');
  // string path, name;
  // if(end_path != string::npos){
  //   path = filename.substr(0, end_path);
  //   name = filename.substr(end_path+1, string::npos);
  // }
  // else{
  //   path = ".";
  //   name = filename;
  // }

  // cout << path << " " << name << endl;

  // return lire(path, name);

  ofstream out("OUT/essai.txt");
  if(!out.is_open()){
    cerr << "erreur" << endl;
    return 0;
  }

  out << "coucou" << endl;
  out.close();

  return 1;

}

  
