#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <iostream>
using namespace std;

#include "./Geometry/transform.h"



class Camera {
public:
  Transform mla; // matrice lookat de pbrt
  float o[4]; // position de la caméra
  float v[4]; // point visé par la caméra
  float up[4]; // vecteur up de la caméra
  float vdir[4]; // vecteur direction de visée
  float fov;
  // dimensions de l'image
  int xresolution;
  int yresolution;
  float ratio; // ratio de l'image = xresolution/yresolution
  // dimensions intiales de le fenêtre d'affichage
  int largeur, hauteur;
  // pas incrémentaux des déplacements
  float dtheta;
  float dtrans;

  Camera();
  ~Camera();

  void initialize(const Transform &m);
  void translate(const float *t);
  void rotateLeft();
  void rotateRight();
  void rotateUp();
  void rotateDown();

  void increaseFov();
  void decreaseFov();

  friend ostream& operator<<(ostream& out, const Camera& cam);

  static  void crossProduct(float* n, float *v1, float *v2);

private:
  void mulMatVec(float m[16], float v[4]);
  void rotate(float a, float b, float c, float ang, float v[4]);
  static const float DTHETA;
  static const float DTRANS;
  static const float DFOV;
  static const int DEFAULT_SCREEN_WIDTH;
  static const int DEFAULT_SCREEN_HEIGHT;
};

#endif
