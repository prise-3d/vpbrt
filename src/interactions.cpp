#include <math.h>
#include <GL/glut.h>
#include "viewpoint.hpp"
#include "Geometry/transform.h"
#include "Camera.hpp"


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define DTHETA 0.1




extern Vector4 vdir;
extern Vector4 pos;
extern Vector4 vup;

extern Camera maCamera;

//void crossProduct(float* n, float *v1, float *v2);


void displayMenu(){ // affichage des touches utilisateur
  // cout << "Touches à utiliser : " << endl;
  // cout << "--------------------" << endl;
  // cout << " w/x  : translation à gauche/à droite" << endl;
  // cout << " up   : avancer" << endl;
  // cout << " down : reculer" << endl;
  // cout << " left : tourner vers la gauche" << endl;
  // cout << " right: tourner vers la droite" << endl;
  // cout << " u/d  : relever/baisser la tête" << endl;
  // cout << " =/+  : descendre/monter" << endl;
  // cout << " a/z  : ouvrir/réduire le fov" << endl;
  // cout << " t/T  : réduire/augmenter le pas de translation" << endl;
  // cout << " r/R  : réduire/augmenter le pas de rotation" << endl;
  // cout << " l/p  : affichage en mode ligne/polygone" << endl;
  // cout << " F2   : sauver la position actuelle" << endl;
  // cout << " q    : quitter" << endl;
  cout << "w/x: left/right translation" << endl;
  cout << "up: moving forward" << endl;
  cout << "down: move back" << endl;
  cout << "left: turn left" << endl;
  cout << "right: turn to the right" << endl;
  cout << "u/d: raise/lower head" << endl;
  cout << "=/+: go down/up" << endl;
  cout << "a/z: open/reduce the fov" << endl;
  cout << "t/T: reduce/increase the translation pitch" << endl;
  cout << "r/R: reduce/increase the rotation step" << endl;
  cout << "r/R: reduce/increase the rotation step" << endl;
  cout << "l/p: display in line/polygon mode" << endl;
  cout << "F2: saving the current position" << endl;
  cout << "q: leave" << endl;
}

void gerer_clavier(unsigned char touche, int x, int y){
  // calcul du vecteur Ox en fontion de l'orientation actuelle
  // on a vx = vdir ^ up avec vdir et up normalisés
  float vx[4];
  Camera::crossProduct(vx, maCamera.vdir, maCamera.up);
  // translation initialisée à nulle
  float t[3]={0.0, 0.0, 0.0};

  switch(touche){
  case 'w' : // on se translate vers la gauche
    // attention : translation inverse car gsScalef -1 selon Ox
    t[0] = vx[0]; t[1] = vx[1]; t[2] = vx[2];
    maCamera.translate(t);
    break;
  case 'x' : // on se translate vers la droite
    // attention : translation inverse car gsScalef -1 selon Ox
    t[0] = -vx[0]; t[1] = -vx[1]; t[2] = -vx[2];
    maCamera.translate(t);
    break;
  case '+': // on monte
    t[0] = maCamera.up[0]; t[1] = maCamera.up[1]; t[2] = maCamera.up[2];
    maCamera.translate(t);
    break;
  case '-': //on descend
    t[0] = -maCamera.up[0]; t[1] = -maCamera.up[1]; t[2] = -maCamera.up[2];
    maCamera.translate(t);    
    break;
  case 'a': //ouvrir le champ de vision
    maCamera.increaseFov();
    break;
  case 'z': //réduire le champ de vision
    maCamera.decreaseFov();
    break;
  case 'r': // réduire la vitesse de rotation
    maCamera.dtheta /= 2.0;
    break;
  case 'R': // accroître la vitesse de rotation
    maCamera.dtheta *= 2.0;
    break;
  case 't': // réduire la vitesse de translation (horizontale et verticale)
    maCamera.dtrans /= 2.0;
    break;
  case 'T': // accroître la vitesse de translation (horizontale et verticale)
    maCamera.dtrans *= 2.0;
    break;
  case 'u': // tourner vers le haut autour de l'axe Ox
    maCamera.rotateUp();
    break;
  case 'd': // tourner vers le bas autour de l'axe Ox
    maCamera.rotateDown();
    break;
  case 'p' :
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case 'l' :
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case 'q': // tout stopper
    exit(0);
    break;
  }

  glutPostRedisplay();

}

void gerer_clavier_special(int touche, int x, int y){
  static const float cosDTheta = cos(DTHETA);
  static const float sinDTheta = sin(DTHETA);


  // vecteur translation
  float t[3]={0.0, 0.0, 0.0};
  
  switch(touche){
  case GLUT_KEY_LEFT :{ // tourner vers la gauche autour de l'axe Oy
    // float vx = vdir.x*cosDTheta + vdir.z*sinDTheta;
    // float vz = -vdir.x*sinDTheta + vdir.z*cosDTheta;
    // vdir.x = vx; vdir.z = vz;
    maCamera.rotateLeft();
    break;
  }
  case GLUT_KEY_RIGHT :{ // tourner vers la droite autour de l'axe Oy
    // float vx = vdir.x*cosDTheta - vdir.z*sinDTheta;
    // float vz = vdir.x*sinDTheta + vdir.z*cosDTheta;
    // vdir.x = vx; vdir.z = vz;
    maCamera.rotateRight();
    break;
  }
  case GLUT_KEY_DOWN : // reculer dans le sens de la direction de visée inverse
    //pos.x -= vdir.x; pos.z -= vdir.z;
    t[0] = -maCamera.vdir[0]; t[1] = -maCamera.vdir[1]; t[2] = -maCamera.vdir[2];
    maCamera.translate(t);
    break;
  case GLUT_KEY_UP : // avancer suivant la direction de visée 
    //pos.x += vdir.x; pos.z += vdir.z;
    t[0] = maCamera.vdir[0]; t[1] = maCamera.vdir[1]; t[2] = maCamera.vdir[2];
    maCamera.translate(t);
    break;
  case GLUT_KEY_F2 :{ // sauvegarder la position de la caméra
    static int numCam = 1;
    ostringstream oss;
    oss << "camera_" << numCam << ".txt";
    string filename(oss.str());
    cout << filename << endl;
    ofstream out(filename);
    if(!out.is_open()){
      cerr << "ouvertue de " << filename << " impossible" << endl;
      break;
    }
    // écriture de la position de la caméra au format pbrt
    out << "LookAt  ";
    out << maCamera.o[0] << " " <<  maCamera.o[1] << " " <<  maCamera.o[2] << "  ";
    out << maCamera.v[0] << " " <<  maCamera.v[1] << " " <<  maCamera.v[2] << "  ";
    out << maCamera.up[0] << " " <<  maCamera.up[1] << " " <<  maCamera.up[2] << endl;
    // écriture du type et de l'ouverture de la caméra
    out << "Camera \"perspective\" \"float fov\" " << maCamera.fov << endl;

    out.close();
    numCam++;
    break;
  }
  }// switch
  
  glutPostRedisplay();
  
}








