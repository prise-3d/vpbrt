#include "PbrtLoader.hpp"
#include "../Geometry/transform.h"
#include "../viewpoint.hpp"
#include "../Camera.hpp"
#include "../Materials/Material.hpp"
#include "../Materials/Glass.hpp"
#include "../Materials/Matte.hpp"
#include "../Materials/Metal.hpp"
#include "../Materials/Mirror.hpp"
#include "../Materials/Plastic.hpp"
#include "../Materials/Uber.hpp"

#include <string>
#include <fstream>
using namespace std;


// extern direction vdir;
// extern position pos;
// extern direction vup;

Vector4 pos={0.0, 0.0, 0.0, 1.0};
Vector4 vdir={0.0, 0.0, 1.0, 0.0};
Vector4 pto={0.0, 0.0, 0.0, 1.0};
Vector4 vup={0.0, 1.0, 0.0, 0.0};
extern Camera maCamera;

#define PRINT() std::cout << str << " : " << std::endl << ctm << std::endl;

PbrtLoader::PbrtLoader(){
  //  pos.x = pos.y = pos.z = 0.0; pos.t = 1.0;
  // cout << pos << " " << vdir << " " << vup << endl; 
  curObj = nullptr;
  curMat = nullptr;
}

PbrtLoader::~PbrtLoader(){
}

// void PbrtLoader::transformer(Vector4 &v){
//   v = ctm * v;
//   v.x = v.x / v.t;
//   v.y = v.y / v.t;
//   v.z = v.z / v.t;
//   v.t = 1;
// }

// void PbrtLoader::transformer(Point3f &s){
//   Vector4 v={s.x, s.y, s.z, 1.0};
//   v = ctm * v;
//   s.x = v.x/v.t; s.y = v.y/v.t; s.z = v.z/v.t;
// }

// ATENTION : la composition de transformations géométriques est
// faite à l'envers dans api.cpp de pbrt - Pour compatibilité, les
// produits matriciels requis sont également effectués
// à l'envers lors du chargement
Scene *PbrtLoader::load(const string &filename){
  // extraction du chemin d'accès
  size_t end_path = filename.find_last_of('/');
  string path;
  if(end_path != string::npos)
    path = filename.substr(0, end_path);
  else
    path = ".";
  
  cout << "loading " << filename << endl;
  
  // lecture du fichier
  ifstream in(filename);

  if(!in.is_open()) return nullptr;

  Scene *scene = new Scene();

  string str; // token de lecture

  str = getToken(in);

  // analyse du fichier pbrt et reconstruction de la scène
  
  while(!in.eof() && str != "WorldEnd"){


    if(str=="WorldBegin"){// réinitialiser ctm
      // a priori, la ctm est supposée contenir les trsansformations nécessaires à la caméra
      // maCamera.mla = ctm;
      maCamera.initialize(ctm);
      // std::cout << maCamera << endl;
      // std::cout << "camera.mla = " << " : " << endl;
      // std::cout << maCamera.mla << std::endl; 
      // réinitialiser la ctm pour les objets
      ctm.SetIdentity();
      // std::cout << "WorldBegin : " << endl;
      // std::cout << ctm << std::endl;
    }
    
    if(str=="AttributeBegin" || str=="TransformBegin"){ 
      // empiler l'état graphique courant - non pris en compte
      // empiler la matrice de transformation courante
      pileMatrices.push(ctm);
      // empiler les bits des transformations actives courants - non pris en compte
      // std::cout << str << " : " << endl;
      // std::cout << ctm << std::endl; 
    }
    
    if(str=="AttributeEnd" || str=="TransformEnd"){
      // restaurer l'état graphique courant - non pris en compte
      // restaure la matrice de transformation courante
      ctm = pileMatrices.top();
      pileMatrices.pop();
      // restaurer les bits des transformations actives courants - non pris en compte
      // std::cout << str << " : " << endl;
      // std::cout << ctm << std::endl; 

    }

    // gestion des transformations géométriques
    if(str=="Scale"){ // ctm = ctm * scale 
      float sx, sy, sz;
      in >> sx >> sy >> sz;
      Transform mscale = Scale(sx, sy, sz);
      // ctm = mscale * ctm;
      ctm = ctm * mscale;
      
      // std::cout << "Scale : " << std::endl;
      // std::cout << ctm << std::endl;
      // std::cout << sx << " " << sy << " " << sz << std::endl;
    }

    if(str=="Translate"){
      float tx, ty, tz;
      in >> tx >> ty >> tz;
      Transform mtrans = Translate(tx, ty, tz);
      //ctm = mtrans * ctm;
      // std::cout << "Translate : avant " << endl;
      // std::cout << ctm << std::endl;
      
      ctm = ctm * mtrans;
      
      // std::cout << "Translate : après " << endl;
      // std::cout << ctm << std::endl;
      
      // std::cout << "Translate : " << endl;
      // std::cout << ctm << std::endl;
    }
    
    if(str=="Rotate"){
      float a, rx, ry, rz;
      in >> a >> rx >> ry >> rz;
      Transform mrot = Rotate(a, rx, ry, rz);
      //      ctm = mrot * ctm;
      // std::cout << "Rotate : avant " << endl;
      // std::cout << ctm << std::endl;

      ctm = ctm * mrot;
      
      // std::cout << "Rotate : après " << endl;
      // std::cout << ctm << std::endl;
    }

    // lecture d'une matrice de transformation
    // ATTENTION : en se basant sur la fonction  pbrtTransform
    // du fichier api.cpp de pbrt, on note que les coefficients
    // de la matrice sont fournis par colonnes successives.
    if(str=="Transform"){
      vector <string> coeff;
      readList(in, coeff);
      Matrix4x4 m;
      for(int c=0; c< 4; c++)// pour chaque colonne
      	for(int l=0; l<4; l++)// pour chaque ligne
      	  m.m[l][c] = stof(coeff[l+c*4]);
      Transform t(m);
      //ctm = t * ctm;
      ctm = ctm * t;

    }
    
    if(str=="ConcatTransform"){
      vector <string> tr;
      if(readList(in, tr) && tr.size()==16){
	ctm = ctm *
	  Transform(Matrix4x4(stof(tr[0]), stof(tr[4]), stof(tr[8]), stof(tr[12]),
			      stof(tr[1]), stof(tr[5]), stof(tr[9]), stof(tr[13]),
			      stof(tr[2]), stof(tr[6]), stof(tr[10]), stof(tr[14]),
		      	      stof(tr[3]), stof(tr[7]), stof(tr[11]), stof(tr[15])));
      }else
	cerr << "ConcatTransform : erreur de lecture de la matrice" << endl;
    }

    if(str=="LookAt"){
      // lecture des attributs de look at
      Vector4 from, to, up;
      in >> from.x >> from.y >> from.z;
      in >> to.x >> to.y >> to.z;
      in >> up.x >> up.y >> up.z;

      from.t = to.t = 1.0; up.t = 0.0;
      
      // Calcul de la matrice de trasformation associée au look at (pbrt)

      Transform mla =
        LookAt(Point3f(from.x, from.y, from.z),
	       Point3f(to.x, to.y, to.z), Vector3f(up.x, up.y, up.z));

      // mise à jour de la matrice de tranformation courante (pbrt)
      ctm = ctm * mla;

      // stockage du vecteur up original dans la caméra
      maCamera.up[0] = up.x;
      maCamera.up[1] = up.y;
      maCamera.up[2] = up.z;

    }

    // tansformations non gérées

    if(str=="CoordinateSystem"){
      str=getToken(in);
      string name = str.substr(1, str.size()-2);
      // recherche si le système existe déjà
      bool existe=false;
      for(int i=0; i<sysCoord.size(); i++)
	if(sysCoord[i]->name == name){
	  sysCoord[i]->ctm = ctm;
	  existe = true;
	  break;
	}
      if(!existe){
	SysCoord *ns = new SysCoord(name, ctm);
	sysCoord.push_back(ns);
      }
    }
	
    if(str=="CoordinateTransform"){
      str=getToken(in);
      string name = str.substr(1, str.size()-2);
      bool existe=false;
      for(int i=0; i<sysCoord.size(); i++)
	if(sysCoord[i]->name == name){
	  ctm = sysCoord[i]->ctm;
	  existe = true;
	  break;
	}
      if(!existe){
	cerr << "système de coordonnées " << name << " inexistant" << endl;
      }
    }    

    
    if(str=="Camera"){ // traiter éventuellement les autres paramètres ...
      str=getToken(in);
      if(str!="\"perspective\"") break; // on ne traite que les caméras perspectives ici
      
      string type, name;
      vector <string> values;
      while(readParameterList(in, type, name, values)){
	if(name=="fov"){
	  maCamera.fov = stof(values[0]);
	}
      }

    }

    if(str=="ObjectBegin"){// lecture d'un objet nommé
      // lecture du nom
      str=getToken(in);
      if(str[0]!='\"'){// erreur - pas de nom au format "nom"
	cerr << "Erreur ObjectBegin - manque le nom au format \"nom\"" << endl;
	in.seekg( -(str.length()), ios_base::cur);
      }else{
	if(curObj){// il existe un autre objet nommé actif non fermé
	  cerr << "Un objet actif nommé " << curObj->getName();
	  cerr << "n'a pas été refermé ..." << endl;
	}
	curObj = new Objet(str.substr(1, str.size()-2));
      }
    }

    if(str=="ObjectEnd"){// fin de lecture d'un objet nommé
      // ajouter l'objet actif à la scène
      scene->add(curObj);
      // désactiver l'objet
      curObj = nullptr;
    }

    if(str=="ObjectInstance"){// lecture d'une instance d'un objet nommé
      // lecture du nom
      str=getToken(in);
      if(str[0]!='\"'){// erreur - pas de nom au format "nom"
	cerr << "Erreur ObjectInstance - manque le nom au format \"nom\"" << endl;
	in.seekg( -(str.length()), ios_base::cur);
      }else{ // ajouter l'instance à la scène
	scene->addInstance(str.substr(1, str.size()-2), ctm);
      }
    }
    
    // traitement d'une forme géométrique
    if(str=="Shape")
      loadShape(in, path, scene, curObj);

    // traitement d'un include
    if(str=="Include"){
      str = getToken(in);
      string filename(path+"/"+str.substr(1, str.size()-2));
      Scene *incScene = load(filename);
      scene->add(incScene);
    }

    if(str=="Film"){
      str = getToken(in);
      if(str!="\"image\""){
	cerr << "paramètre " << str << " invalide pour le film" << endl;
      }else{// lecture des attributs
	string type, name;
	vector <string> values;
	
	while(readParameterList(in, type, name, values)){// decoder les paramètres
	  if(name == "xresolution") maCamera.xresolution = stoi(values[0]);
	  if(name == "yresolution") maCamera.yresolution = stoi(values[0]);
	  // le reste n'est pas pris en charge dans cette version
	  
	  // raz du tampon de paramètres
	  values.clear();
	}
	// maj à jour des dimensions de la fenêtre d'affichage
	float ratiox = maCamera.xresolution / 800.0;
	float ratioy = maCamera.yresolution / 800.0;
	float maxratio = (ratiox>ratioy) ? ratiox : ratioy;
	if(maxratio > 1.0){
	  maCamera.largeur = (int)(maCamera.xresolution/maxratio);
	  maCamera.hauteur = (int)(maCamera.yresolution/maxratio);	  
	}else{
	  maCamera.largeur = maCamera.xresolution;
	  maCamera.hauteur = maCamera.yresolution;
	}
	maCamera.ratio = (float)maCamera.largeur/(float)maCamera.hauteur;

      }// else
	
    }// Film

    if(str=="MakeNamedMaterial"){// créer un matériau nommé
      // récupérer le nom du matériau de la forme "nom"
      string name = getToken(in);
      
      // récupérer les paramètres du matériau
      string type, paramName;
      vector <string> values;
      ParamSet set;


      while(readParameterList(in, type, paramName, values)){// decoder les paramètres
	set.addParameters(type, paramName, values);
	values.clear();
      }

      // créer le matériau
      Material *mat = createMaterial(name.substr(1, name.size()-2), set);
            
      // ajouter le matériau à la liste des matériaux nommés
      // on ne vérifie pas si le matériaux existe déjà
      materials.push_back(mat);

    }

    if(str=="NamedMaterial"){ // assigner le nom du matériau courant
      // récupérer le nom du matériau de la forme "nom"
      string name = getToken(in);
      name = name.substr(1, name.size()-2);
      // recherche le matériau dans la liste via son nom
      bool trouve = false;
      for(int i=0; i<materials.size(); i++)
	if(materials[i]->getName() == name){
	  curMat = materials[i];
	  trouve=true;
	  break;
	}
      if(!trouve){
	std::cout << "material " << name << "not found" << endl;
	curMat = nullptr;
      }
    }

    // traitement d'un commentaire
    // if(str[0]=='#') getline(in, str);
      
    str=getToken(in);

  }
  

  in.close();
  // cerr << ctm << endl;

  // for(int i=0; i<materials.size(); i++)
  //   cout << *(materials[i]) << endl;
  
  return scene;
}



// traitement du chargement d'une forme
void PbrtLoader::loadShape(ifstream &in, const string& path, Scene *scene, Objet *cur){
  string str; // token de lecture
				  
  // lecture du type de forme
  str = getToken(in);

  // traitement des formes
  if(str=="\"plymesh\"") loadPlymesh(in, path, scene, cur);
  else if(str=="\"trianglemesh\"") loadTriangleMesh(in, path, scene, cur);
  else if(str=="\"cylinder\"") loadCylinder(in, path, scene, cur);

  // sinon on passe dans la boucle principale
  
  return;
  
}


// chargement d'un mesh au format PLY
void PbrtLoader::loadPlymesh(ifstream &in, const string& path, Scene *scene, Objet *cur){
  string type, name;
  vector <string> values;
  
  if(!readParameterList(in, type, name, values)){
    std::cout << "erreur de format pour forme Shape trianglemesh" << std::endl;
    return;
  }

  
  if(name=="filename"){// récupération du nom de fichier
    string filename = path+"/";
    filename += (values[0][0]=='\"') ? values[0].substr(1, values[0].size()-2) : values[0];
    // cout << "ply : " << filename << endl;
    if(!scene->load_ply(filename.c_str(), ctm, cur, curMat)){
      cout << "error loading " << filename << endl;
      cout << "path = " << path << endl;
    }
    return;
  }	
  

  // traiter également
  // "float alpha" [ 0.0 ]
  // "float alpha  0.0
  // "texture alpha" [ "filename" ]
  // "texture alpha" "filename"
  // plus compliqué, car les paramètres peuvent être absents ou présents
  // dans n'import quel ordre. Du coup on peut être amené à lire un cran
  // trop loin dans le fichier ...

}// plymesh

void PbrtLoader::loadTriangleMesh(ifstream &in, const string& path, Scene *scene, Objet *cur){
  string type, name;
  vector <string> values;
  
  Mesh *mesh = new Mesh();
  // ajout du matériel courant
  mesh->setMaterial(curMat);

  //  cout << "triangles : " << endl;

  while(readParameterList(in, type, name, values)){
    if(name=="indices"){// traiter les indices des sommets des triangles
      Face tri;
      for(int i=0; i< values.size(); i+=3){ // on traite des triangles
	tri.isom[0] = stoi(values[i]);
	tri.isom[1] = stoi(values[i+1]);
	tri.isom[2] = stoi(values[i+2]);
	mesh->add(tri);
      }
    }else if(name=="P"){// traiter les coordonnées des sommets
      Point3f s;
      for(int i=0; i< values.size(); i+=3){ // on traite des (x, y, z)
	s.x = stof(values[i]);
	s.y = stof(values[i+1]);
	s.z =stof( values[i+2]);
	s.transformer(ctm);// appliquer la ctm sur les coordonnées
	mesh->add(s);
      }
    }else if(name=="N"){// traiter les normales aux sommets
    }else if(name=="S"){// traiter les tangentes aux sommets
    }else if(name=="uv" || name=="st"){// traiter les coordonnées de texture aux sommets
    }else if(name=="alpha"){
    }else if(name=="shadowalpha"){
    }else cerr << "erreur paramètre " << name << " de trianglemesh inconnu" << endl;
	

    values.clear();
  }

  // la transformation a été appliquée au moment du chargement ...
  if(!cur) // pas d'objet courant - ajout direct à la scène
    scene->add(mesh);
  else
    cur->addMesh(mesh);
  
  return;
	
}

// chargement d'un cylindre

void PbrtLoader::loadCylinder(ifstream &in, const string& path, Scene *scene, Objet *cur){
  string type, name;
  vector <string> values;
  float rayon = 1, zmin =-1, zmax = 1;

  while(readParameterList(in, type, name, values)){
    if(name=="radius"){// récupérer le rayon
      rayon = stof(values[0]);
    }else if(name=="zmin"){// récupérer la base du cylindre
      zmin = stof(values[0]);
    }else if(name=="zmax"){// réucpérer le haut du cylindre
      zmax = stof(values[0]);
    }else cerr << "cylindre : paramètre " << name << "inconnu" << endl;
    values.clear();

  }// while
  
  Cylindre *cyl = new Cylindre(rayon, zmin, zmax);
  
  // appliquer la transformation courante
  cyl->transform(ctm);


  if(!cur) // pas d'objet courant - ajout direct à la scène
    scene->add(cyl);
  else
    cur->addCylindre(cyl);
}


// Lecture d'un triplé représentant une liste de paramètres au format pbrt
// La liste a toujours le format  "type  name" [ value ou values ]
// Si il n'y a qu'une seule valeur, on peut avoir "type name"  value
// Au cas où la lecture demandée ne serait pas une liste de paramètres, la méthode
// laisse le fichier tel qu'il était avant la lecture et renvoie faux.
bool PbrtLoader::readParameterList(ifstream & in, string &type, string &name, vector<string> &values){
  string str;// tampon de lecture
  
  str=getToken(in); // lecture du type potentiel

  // vérifier qu'il y avait encore des choses à lire
  if(in.eof()) return false;
  
  if(str[0]!='\"'){// on a pas trouvé un type
    // on se remet avant la lecture pour permettre celle-ci à l'extérieur de la méthode
    in.seekg( -(str.length()), ios_base::cur);
    return false;
  }

  // on a trouvé un type
  type = str.substr(1);
  // on lit le nom
  str = getToken(in);
  name = str.substr(0, str.length()-1);

  // lire la ou les valeurs
  str=getToken(in);
  // est-ce une liste ?
  if(str[0]=='['){// c'est une liste
    if(str.length() > 1)// on est dans le cas [xxx
      str = str.substr(1);
    else str=getToken(in);
    // str est de la forme xxx ou xxx]
    do{
      if(str[str.length()-1]!=']'){ // forme xxx
	values.push_back(str);
	str=getToken(in);
      }else{ // forme xxx] ou ]
	if(str.length()>1){// forme xxx]
	  str = str.substr(0, str.length()-1);
	  values.push_back(str);
	}
	break;
      }
    }while(1);
  }else // valeur unique
    values.push_back(str);

  return true;
}

// lecture d'une liste qui est au format :
// - xxx si un seul élément
// - [ xxx ...] si >= 1 élément
bool PbrtLoader::readList(ifstream & in, vector<string> &values){
  string str;// tampon de lecture
  
  // lire la ou les valeurs qui est forcément présente
  str=getToken(in);
  // est-ce une liste ?
  if(str[0]=='['){// c'est une liste
    if(str.length() > 1)// on est dans le cas [xxx
      str = str.substr(1);
    else str=getToken(in);
    // str est de la forme xxx ou xxx]
    do{
      if(str[str.length()-1]!=']'){ // forme xxx
	values.push_back(str);
	str=getToken(in);
      }else{ // forme xxx] ou ]
	if(str.length()>1){// forme xxx]
	  str = str.substr(0, str.length()-1);
	  values.push_back(str);
	}
	break;
      }
    }while(1);
  }else // valeur unique
    values.push_back(str);

  return true;
}

// lecture d'un mot du flux d'entrée, en passant les lignes
// commençant par un # (commentaire)
string PbrtLoader::getToken(ifstream &in){
  string str;
  in >> str;
  while((!in.eof()) && (str[0]=='#')){
    getline(in, str);
    in >> str;
  }
    
  return str;
}


Material *PbrtLoader::createMaterial(const string &name, const ParamSet &set){
  vector <string> type = set.getValuesFor("type");

  if(type.size()==0) return new Matte(name);
  
  // if(type=="disney") return new Disney(name, set);
  // if(type=="fourier") return new Fourier(name,set);;
  if(type[0]=="\"glass\"") return new Glass(name, set);
  // if(type=="hair") return new Hair(name, set);
  // if(type=="kdsubsurface") return new KdSubsurface(name, set);
  if(type[0]=="\"matte\""){
    Matte *m = new Matte(name, set);
    cout << *m << endl;
    return m;
  }
  if(type[0]=="\"metal\"") return new Metal(name, set);
  if(type[0]=="\"mirror\""){
    Mirror *m = new Mirror(name, set);
    cout << *m << endl;
    return m;
  }
  // if(type=="mix") return new Mix(name, set);
  // if(type=="none") return NONE;
  if(type[0]=="\"plastic\"") return new Plastic(name, set);
  // if(type=="substrate") return new Substrate(name, set);
  // if(type=="subsurface") return new Subsurface(name, set);
  // if(type=="translucent") return new Translucent(name, set);
  if(type[0]=="\"uber\"") return new Uber(name, set);
  return new Matte(name);
}
