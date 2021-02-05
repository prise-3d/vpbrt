#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <sstream>
using namespace std;
#include "rply-1.1.4/rply.h"
#include "Geometry/Mesh.hpp"
#include "Geometry/Scene.hpp"
#include "IO/PbrtLoader.hpp"
#include "graphique.hpp"
#include "interactions.hpp"
#include "Camera.hpp"
#include "Chemin.hpp"

#define VERSION "1.5"
// Ajouts 1.5 :
// - chargement de chemins pour visu dans la scène
// TODO :
// - améliorer la gestion du nb de faces
// - appliquer les ltm sur les instances sur GPU plutôt que sur CPU
// - Intégrer les matériaux



// variables globales

Scene *curScene; // la scène chargée
PbrtLoader curloader; // le "chargeur" de scène au format pbrt
Camera maCamera; // la camera active
int curPath; // indice du chemin actif
bool viewCylinder; // drapeau d'activation de la visu des cylindres pbrt

std::vector<Chemin> chemins;

bool extractArg(int argc, char *argv[],
		std::string &pbrtName, std::string &pathDirName);
void loadPaths(std::string pathDirName, std::vector<Chemin> &chemins);
// bool getPath(ifstream &in, Chemin &path);
// void printPath(const Chemin &path);
bool isPathFile(std::string filename);



void printPaths(const std::vector<Chemin> &chemins){
  std::cout << "------ CHEMINS ------" << std::endl;
  for(int i=0; i<chemins.size(); i++)
    std::cout << "p" << i << " : " << chemins[i] << std::endl;
  std::cout << "------ CHEMINS ------" << std::endl;

}


// initialisation des paramètres de la caméro opengl
static void init_screen(void){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(maCamera.fov, (float)maCamera.largeur/(float)maCamera.hauteur,
		 0.1, 1000.0 );

  glViewport(0,0, maCamera.largeur, maCamera.hauteur);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

int main(int argc, char *argv[]) {
  std::string pbrtName, pathDirName;

  if(!extractArg(argc, argv, pbrtName, pathDirName)) return -1;

  std::cout << pbrtName << " - " << pathDirName << std::endl;

  printf("======== vpbrt version %s ======== \n", VERSION);

  // chargement de la scène
  curScene=curloader.load(pbrtName);
  if(!curScene){
    printf("Error when loading file %s\n", argv[1]);
    return -1;
  }else {
     curScene->printStats();
     viewCylinder = false;
     //curScene->printCylinders();
  }

  cout << "dim image = " << maCamera.largeur << " " << maCamera.hauteur << endl;

  // chargement des chemins à visualiser
  loadPaths(pathDirName, chemins);
  //printPaths(chemins);

  
  // initialisation de glut
  glutInit (&argc, argv);

  glutInitWindowPosition(100, 100); 
  //  glutInitWindowSize(maCamera.largeur, maCamera.hauteur); 
  glutInitWindowSize(960, 540); 

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow(argv[0]);
  
  // choix des fonctions callback pour glut 
  glutDisplayFunc(dessiner);
  glutReshapeFunc(retailler);
  glutKeyboardFunc(gerer_clavier);
  glutSpecialFunc(gerer_clavier_special);

  // initialisation des paramètres d'affichage
  init_screen();
  // affichage du menu des commandes
  displayMenu();

  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  
  return 0;
}



// syntaxe : ./vpbrt -f <file.pbrt> [-d <pathdir>]

bool extractArg(int argc, char *argv[],
		std::string &pbrtName, std::string &pathDirName){
  // vérification du nombre d'arguments ninimal
  if(argc <3){
    std::cout << "syntax: " << argv[0] << " -f <file.pbrt> ";
    std::cout << "[ -d <file.path>+]" << std::endl;
    std::cout << "with:" << std::endl;
    std::cout << "<file.pbrt> the name of the pbrt file" << std::endl;
    std::cout << "<file.path> the name of the file containing the eay paths (format csv)" << std::endl;
    return false;
  }
  
  // décodage des arguments
  for(int i=1; i<argc; i++){
    if(strcmp(argv[i],"-f")==0){ // fichier pbrt
      if(i+1==argc){
	std::cout << "option -f : missing pbrt file" << std::endl;
	return false;
      }else {
	pbrtName = std::string(argv[i+1]);
	i++;
      }
    }else if(strcmp(argv[i],"-d")==0){// dossier contenant les chemins
      if(i+1==argc){
	std::cout << "option -d : missing folder with paths" << std::endl;
	return false;
      }else{
	pathDirName = argv[i+1];
	i++;
      }
    } else {
      std::cout << "unknown " << argv[i] << " option" << std::endl;
      return false;
    }
  }


  return true;
	
}


void loadPaths(std::string pathDirName, std::vector<Chemin> &chemins){
  curPath = -1;

  // ouverture du dossier contenu les fichiers "chemin"
  DIR *dir = opendir (pathDirName.c_str());
  if(!dir){
    std::cout << "error when opening folder " << pathDirName << std::endl;
    return;
  }

  // récupération des fichiers chemin
  struct dirent* entry;
  
  while ((entry = readdir (dir)) != NULL) {
    string filename = pathDirName+"/"+entry->d_name;
    struct stat st;
    lstat(filename.c_str(), &st);
    if(S_ISREG(st.st_mode) && isPathFile(filename)){
       std::cout << "-" << filename << std::endl;
    // lire un chemin
       Chemin path;
       ifstream in(filename.c_str());
       if(!in.is_open()){
          std::cout << "reading error " << filename << std::endl;
        }else{

          std::string line;
          while(path.readPath(in)){
            // std::cout << path << std::endl;
            chemins.push_back(path);
            path.clear();
          }
        in.close();
       }
	 
    }
  }// while
  
  closedir(dir);

  if(chemins.size()>0) curPath = 0;
}



// on vérifie ici juste que l'extension du nom de fichier
// est bien un csv - sans doute prévoir un "magic number" en entête
// pour vérifier indépendamment du nom de fichier
bool isPathFile(std::string filename){
  size_t pos = filename.rfind(".csv"); // recherche de la dernière occurrence
  if(pos!=filename.length()-4) return false;
  return true;
}


