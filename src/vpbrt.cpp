#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <fstream>
using namespace std;
#include "rply-1.1.4/rply.h"
#include "Geometry/Mesh.hpp"
#include "Geometry/Scene.hpp"
#include "IO/PbrtLoader.hpp"
#include "graphique.hpp"
#include "interactions.hpp"
#include "Camera.hpp"

#define VERSION "1.3"
// Corrections 1.3 :
// - prise en charge des systèmes de coordonnées nommés (non testés, car aucun des
//   fichiers  pbrt disponibles ne les utilise)
// - ajout de la possibilité de baisser ou remonter la direction de visée de la caméra
// - ajout de la gestion des shapes de type trianglemesh
// - ajout d'un changement de couleur d'affichage en cas de sortie de la bounding box
//   de la scène.
// TODO :
// - améliorer la gestion du nb de faces
// - appliquer les ltm sur les instances sur GPU plutôt que sur CPU
// - Intégrer les matériaux


// variables globales

Scene *curScene; // la scène chargée
PbrtLoader curloader; // le "chargeur" de scène au format pbrt
Camera maCamera; // la camera active


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

  // vérification des paramètres de la ligne de commandes
  if(argc!=2){
    printf("Syntaxprfile.pbrt>\n", argv[0]);
    return -1;
  }

  printf("===== vpbrt version %s =====\n", VERSION);

  // chargement de la scène
  curScene=curloader.load(argv[1]);
  if(!curScene){
    printf("Error when loading file: %s\n", argv[1]);
    return -1;
  }else
    curScene->printStats();


  // initialisation de glut
  glutInit (&argc, argv);

  glutInitWindowPosition(100, 100); 
  glutInitWindowSize(maCamera.largeur, maCamera.hauteur); 

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
