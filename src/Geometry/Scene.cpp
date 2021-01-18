#include "Scene.hpp"
#include <fstream>
using namespace std;

// variables globales nécessaires pour les fonctions
// call back de chargement des ply
Mesh *tmpMesh=nullptr; // le mesh courant
Transform curCtm; // la matrice de transformation courante


Scene::Scene(){
  nbFaces = nbSommets = 0;
}

Scene::~Scene(){
  objets.clear();
  ltm.clear();
  meshes.clear();
}

void Scene::updateBB(const BoundingBox &bbp){
  if(bb.min.x > bbp.min.x) bb.min.x = bbp.min.x;
  if(bb.min.y > bbp.min.y) bb.min.y = bbp.min.y;
  if(bb.min.z > bbp.min.z) bb.min.z = bbp.min.z;
  if(bb.max.x < bbp.max.x) bb.max.x = bbp.max.x;
  if(bb.max.y < bbp.max.y) bb.max.y = bbp.max.y;
  if(bb.max.z < bbp.max.z) bb.max.z = bbp.max.z;
}

void Scene::add(Mesh *m){
  // ajout d'un nouveau mesh sans transformations
  // ces transformations sont censées avoir été faites
  // lors du chargement du mesh
  Transform *t = new Transform();
  t->SetIdentity();
  ltm.push_back(t);
  meshes.push_back(m);

  // màj des stats de la scène
  nbFaces += m->getNbFaces();
  nbSommets += m->getNbSommets();

  // maj de la BB de la scène
  updateBB(m->getBB());
}


void Scene::add(Mesh *m, Transform *t){
  // ajout du mesh et de sa transformation locale associée
  ltm.push_back(t);
  meshes.push_back(m);
  // màj des stats de la scène
  nbFaces += m->getNbFaces();
  nbSommets += m->getNbSommets();
  
  // maj de la BB de la scène
  BoundingBox lbb = m->getBB();
  lbb.min.transformer(*t);
  lbb.max.transformer(*t);
  updateBB(lbb);
}


void Scene::add(Objet *o){
  // ajouter un objet nommé
  if(o) objets.push_back(o);
}


void Scene::addInstance(const string &oname, const Transform &t){
  // rechercher l'objet nommé duquel il faut récupérer les meshes
  Objet *o = nullptr;
  for(int i=0; i<objets.size(); i++)
    if(objets[i]->getName() == oname){
      o = objets[i];
      break;
    }

  // vérifier l'existence de l'objet nommé
  if(!o){
    cerr << "instanciation : l'objet " << oname << " n'existe pas" << endl;
    return;
  }

  // ajout des meshes de l'objet dans la scène
  vector <Mesh *> om = o->getMeshes();
  
  for(int i=0; i<om.size(); i++)
    add(om[i], new Transform(t));

}

void Scene::add(Scene *sc){
  // ajout des objets nommés
  for(int i=0; i<sc->objets.size(); i++){
    objets.push_back(sc->objets[i]);
    sc->objets[i] = nullptr; // evite le partage du mesh
  }
  
  // ajout des meshes visibles et leur ltm
  for(int i=0; i<sc->meshes.size(); i++){
    ltm.push_back(sc->ltm[i]);
    meshes.push_back(sc->meshes[i]);
  }

  // màj des stat de la scène
  nbFaces += sc->nbFaces;
  nbSommets += sc->nbSommets;

    // maj de la BB de la scène
  updateBB(sc->getBB());
}



ostream& operator<<(ostream &out, const Scene &sc){

  BoundingBox bb ={{1e10, 1e10, 1e10}, {-1e10, -1e10, -1e10}};
  
  out << "--------- BEGIN SCENE ----------" << endl;

  out << "=== Objets nommés ===" << endl;
  
  for(int i=0; i<sc.objets.size(); i++)
    out << sc.objets[i]->getName();

  out << "=== Meshes ===" << endl;

  for(int i=0; i<sc.meshes.size(); i++){
    out << *(sc.meshes[i]);
  }
    // calcul de la bounding box de la scène
  //   BoundingBox bbm = sc.objets[i]->getBB();
  //   if(bbm.min.x < bb.min.x) bb.min.x = bbm.min.x;
  //   if(bbm.min.y < bb.min.y) bb.min.y = bbm.min.y;
  //   if(bbm.min.z < bb.min.z) bb.min.z = bbm.min.z;
  //   if(bbm.max.x > bb.max.x) bb.max.x = bbm.max.x;
  //   if(bbm.max.y > bb.max.y) bb.max.y = bbm.max.y;
  //   if(bbm.max.z > bb.max.z) bb.max.z = bbm.max.z;
  // }

  
  out << "-------- Bounding Box --------" << endl;
  out << "(" << bb.min.x << "," << bb.min.y << "," << bb.min.z << ")";
  out << " - (" << bb.max.x << "," << bb.max.y << "," << bb.max.z << ")";
  out << endl;
  out << "--------- END SCENE ----------" << endl;
  
  return out;
}


// static void transformer(Point3f &s){
//   Vector4 v(s.x, s.y, s.z, 1);
//   v = curCtm * v;
//   v.x = v.x / v.t;
//   v.y = v.y / v.t;
//   v.z = v.z / v.t;
//   s.x = v.x; s.y = v.y; s.z = v.z;
// }

static int vertex_cb(p_ply_argument argument) {
  long eol;
  float value;
  p_ply_property property;
  const char *name;
  static Point3f stmp;
    
  ply_get_argument_user_data(argument, NULL, &eol);
  value = ply_get_argument_value(argument);
  ply_get_argument_property(argument, &property, NULL, NULL);
  ply_get_property_info(property, &name, NULL, NULL, NULL);
  string sname(name);
  if(sname=="x")
    stmp.x = value;
  else if(sname=="y")
    stmp.y = value;
  else if(sname=="z"){
    stmp.z = value; // repère pbrt à main gauche - changer le signe ?
    stmp.transformer(curCtm);
    tmpMesh->add(stmp);
  }

  return 1;
}

static int face_cb(p_ply_argument argument) {
    long length, value_index;
    static Face ftmp;
    int index;
    
    ply_get_argument_property(argument, NULL, &length, &value_index);
    index = ply_get_argument_value(argument);
    switch (value_index) {
    case 0: ftmp.isom[0] = index; break;
    case 1: ftmp.isom[1] = index; break;
    case 2: ftmp.isom[2] = index; tmpMesh->add(ftmp); break;
    default:
      break;
    }
    return 1;
}

bool Scene::load_ply(const char *filename, const Transform &ctm, Objet *cur){

  curCtm = ctm;

  p_ply ply = ply_open(filename, NULL, 0, NULL);
  if (!ply) return false;
  if (!ply_read_header(ply)) return false;
  
  long nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
  ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
  ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
    
  long ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);


  tmpMesh = new Mesh();

  if (!ply_read(ply)){
    delete tmpMesh;
    return false;
  }
  ply_close(ply);

  if(!cur) // pas d'objet courant - ajout direct à la scène
    add(tmpMesh);
  else
    cur->addMesh(tmpMesh);

  
  return true;
  
}

/**
 * affichage opengl de la scène en prenant en compte sa complexité (solution tmp)
 * - on tient compte du nombre total de faces  présentes dans la scène
 * - si ce nombe est inférieur à 10 000 000 de faces, elles sont toutes affichées
 * - sinon on vérifie meshe par mesh et on affiche les faces des mesh de taille
 * - inférieure à 100. Pour les autres on affiche leur BB.
 */
void Scene::draw(){
 
  if(nbFaces < 10000000){
    for(unsigned int i=0; i<meshes.size(); i++)
      if(ltm[i]->IsIdentity())
	meshes[i]->draw();
      else
	meshes[i]->draw(*(ltm[i]));
  }else{
    for(unsigned int i=0; i<meshes.size(); i++)
      if(ltm[i]->IsIdentity()){
	if(meshes[i]->getNbFaces() < 100)
	  meshes[i]->draw();
	else
	  meshes[i]->drawBB();
      }else{
	if(meshes[i]->getNbFaces() < 100)
	  meshes[i]->draw(*(ltm[i]));
	else
	  meshes[i]->drawBB(*(ltm[i]));
      }
  }
  
}


bool Scene::isInsideBB(const Point3f &p){
  if(p.x < bb.min.x || p.x > bb.max.x) return false;
  if(p.y < bb.min.y || p.y > bb.max.y) return false;
  if(p.z < bb.min.z || p.z > bb.max.z) return false;
  return true;
}


void Scene::printStats(){
  cout << "STATISTIcS : " << endl;
  cout << " - nb faces = " << nbFaces << endl;
  cout << " - nb vectices = " << nbSommets << endl;
  cout << " - nb meshes = " << meshes.size() << endl;
  cout << "Mean number of faces / mesh = " << nbFaces/meshes.size() << endl;
}

