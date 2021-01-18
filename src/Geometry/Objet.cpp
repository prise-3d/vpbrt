#include "Objet.hpp"

Objet::Objet(){
}

Objet::Objet(const string &name){
  this->name = name;
}

Objet::~Objet(){
  // ltm.clear();
  meshes.clear();
}

const string &Objet::getName(){
  return name;
}

vector <Mesh*> Objet::getMeshes(){
  return meshes;
}

void Objet::addMesh(Mesh *m){
  if(m) meshes.push_back(m);
}

void Objet::addMesh(Mesh *m, const Transform &t){
  if(m){
    meshes.push_back(m);
  }
}

BoundingBox Objet::getBB(){
  BoundingBox bb ={{1e10, 1e10, 1e10}, {-1e10, -1e10, -1e10}};

  for(unsigned int i=0; i<meshes.size(); i++){
    // màj de la bounding box de l'objet
    BoundingBox bbm = meshes[i]->getBB();
    if(bbm.min.x < bb.min.x) bb.min.x = bbm.min.x;
    if(bbm.min.y < bb.min.y) bb.min.y = bbm.min.y;
    if(bbm.min.z < bb.min.z) bb.min.z = bbm.min.z;
    if(bbm.max.x > bb.max.x) bb.max.x = bbm.max.x;
    if(bbm.max.y > bb.max.y) bb.max.y = bbm.max.y;
    if(bbm.max.z > bb.max.z) bb.max.z = bbm.max.z;
  }

  return bb;
}

// normalement inutile, car les objets sont invisibles
void Objet::draw(){

  for(unsigned int i=0; i<meshes.size(); i++)
    meshes[i]->draw();

}

ostream& operator<<(ostream &out, const Objet &obj){
  
  out << "---------- objet " << obj.name << "-----------" << endl;
  for(unsigned int i=0; i<obj.meshes.size(); i++){
    out << "mesh " << i << endl;
    out << *(obj.meshes[i]) << endl;
  }

  return out;
}
