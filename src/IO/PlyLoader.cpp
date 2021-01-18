#include "PlyLoader.hpp"

PlyLoader::PlyLoader(){
  nbSommets = nbFaces =  0;
  format = PLY_ASCII;
}

PlyLoader::~PlyLoader(){
}

bool PlyLoader::load(const string& name){
  // ouverture du fichier
  ifstream in;

  in.open(name,ios::binary);

  if(!in.is_open()){
    cerr << "error opening " << name << endl;
    return false;
  }

  // lecture de l'entête
  if(!readHeader(in)){
    in.close();
    return false;
  }
  

  // lecture des données
  if(!readData(in)){
    in.close();
    return false;
  }

  in.close();

  return true;
  
}

bool PlyLoader::readHeader(ifstream &in){
  string str, ligne;
  bool propSom = false; // on a pa lu les propriétés des sommets
  bool propFac = false; // on a pa lu les propriétés des faces

  getline(in, ligne);;
  if(ligne!="ply"){
    cerr << "Not a ply file" << endl;
    return false;
  }

  in >> str;
  
  while(str != "end_header"){
    if(str=="format"){
      in >> str;
      if(str=="ascii") format= PLY_ASCII;
      else if(str=="binary_little_endian") format = PLY_LITTLE_ENDIAN;
      else if(str=="binary_big_endian") format = PLY_BIG_ENDIAN;
      else {
	cerr << "Unknown data format : " << str << endl;
	return false;
      }
      in >> version;
    }// format
    if(str=="comment") getline(in, ligne);
    if(str=="element"){
      in >> str;
      if(str=="vertex"){
	in >> nbSommets;
	propSom = true;
      }
      else if(str=="face"){
	in >> nbFaces;
	propFac = true; propSom = false;
      }
      else {
	cerr << "Unknown element : " << str << endl;
	return false;
      }
    }// element
    if(str=="property"){
      if(propSom){// on est dans la partie décrivant les propriétés des sommets
	in >> str >> str; // type - nom
	propNames.push_back(str);
      }else{// on est dans la partie décrivant les propriétés des faces
	getline(in, ligne); // list uint8 int vertex_indices
      }
    }
    in >> str;
  }// while
	

  return true;
}

bool PlyLoader::readData(ifstream &in){

  return true;
}

void PlyLoader::print(){
  cout << "ply : " << endl;
  cout << "format : " <<  format << " " << version << endl;
  cout << "nb sommets = " << nbSommets << endl;
  cout << "nb Faces = " << nbFaces << endl;
  
}
