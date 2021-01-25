#include "Camera.hpp"

//#define DEBUG

const float Camera::DTHETA = 0.1;
const float Camera::DTRANS = 0.1;
const float Camera::DFOV = 2.0;
const int Camera::DEFAULT_SCREEN_WIDTH = 512;
const int Camera::DEFAULT_SCREEN_HEIGHT = 512;

Camera::Camera(){
  o[0]=o[1]=o[2]=0.0; o[3]=1.0;
  v[0]=0; v[1]=0; v[2]=1; v[3]=1;
  up[0]=0; up[1]=1; up[2]=0; up[3]=1;
  vdir[0]=0; vdir[1]=0; vdir[2]=1.0; vdir[3]=0.0;
  fov=30.0;

  xresolution = largeur = DEFAULT_SCREEN_WIDTH;
  yresolution = hauteur = DEFAULT_SCREEN_HEIGHT;
  ratio = (float)xresolution/(float)yresolution;

  dtheta = DTHETA;
  dtrans = DTRANS;
}

Camera::~Camera(){
}

// initialisation de la position de la caméra
void Camera::initialize(const Transform &m){
  mla = m;
  // retrouver la position de la caméra
  mulMatVec((float*)(m.GetInverseMatrix().m), o);
  // retrouver le point visé
  mulMatVec((float*)(m.GetInverseMatrix().m), v);
#ifdef UP
  // si on applique ce code, on essaie de retrouver le vecteur up
  // qui doit correspondre à (0,1,0) dans le repère de la caméra
  // Il est donc modifié si la direction de visée n'est pas horizontale
  
  // retrouver le vecteur up et le normaliser
  mulMatVec((float*)(m.GetInverseMatrix().m), up);
  for(int i=0; i<4; i++) up[i] -= o[i];
  float l = std::sqrt(up[0]*up[0]+up[1]*up[1]+up[2]*up[2]);
  for(int i=0; i<3; i++) up[i] /= l;
#endif
  // calculer le vecteur direction de visée et le normaliser
  for(int i=0; i<4; i++) vdir[i] = v[i] - o[i];
  float lg = std::sqrt(vdir[0]*vdir[0]+vdir[1]*vdir[1]+vdir[2]*vdir[2]);
  for(int i=0; i<3; i++) vdir[i] /= lg;
}

void Camera::translate(const float *t){

  for(int i=0; i<3; i++){
    o[i] += t[i]*dtrans;
    v[i] += t[i]*dtrans;
  }

}

 // tourner vers la gauche autour de l'axe up
void Camera::rotateLeft(){

  rotate(up[0], up[1], up[2], -dtheta, vdir);
  // mise à jour du point visé
  for(int i=0; i<3; i++)
    v[i] = o[i]+vdir[i];
}

 // tourner vers la droite autour de l'axe up
void Camera::rotateRight(){
  // calcul de rotation inversé car glScalef -1 selon Ox
  rotate(up[0], up[1], up[2], dtheta, vdir);
  // mise à jour du point visé
  for(int i=0; i<3; i++)
    v[i] = o[i]+vdir[i];
}


void Camera::rotateUp(){
  // calcul du vecteur Ox en fontion de l'orientation actuelle
  // on a vx = vdir ^ up avec vdir et up normalisés
  float vx[4];
  crossProduct(vx, vdir, up);

  rotate(vx[0], vx[1], vx[2], dtheta, vdir);
  // mise à jour du point visé
  for(int i=0; i<3; i++)
    v[i] = o[i]+vdir[i];
  
}

void Camera::rotateDown(){
  // calcul du vecteur Ox en fontion de l'orientation actuelle
  // on a vx = vdir ^ up avec vdir et up normalisés
  float vx[4];
  crossProduct(vx, vdir, up);

  rotate(vx[0], vx[1], vx[2], -dtheta, vdir);
  // mise à jour du point visé
  for(int i=0; i<3; i++)
    v[i] = o[i]+vdir[i];
  
}

ostream& operator<<(ostream& out, const Camera& cam){
  out << "Camera : matrice = " << endl;
  out << cam.mla << endl;
  out << "Camera : LookAt ";
  for(int i=0; i<3;i++) out << cam.o[i] << " ";
  out << "  ";
  for(int i=0; i<3;i++) out << cam.v[i] << " ";
  out << "  ";
  for(int i=0; i<3;i++) out << cam.up[i] << " ";
  out << endl;
  
  return out;
}

void Camera::mulMatVec(float m[16], float v[4]){
  float v0, v1, v2, v3;
  v0 = m[0]*v[0]+m[1]*v[1]+m[2]*v[2]+m[3]*v[3];
  v1 = m[4]*v[0]+m[5]*v[1]+m[6]*v[2]+m[7]*v[3];
  v2 = m[8]*v[0]+m[9]*v[1]+m[10]*v[2]+m[11]*v[3];
  v3 = m[12]*v[0]+m[13]*v[1]+m[14]*v[2]+m[15]*v[3];
  v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;
}


void Camera::increaseFov(){
  fov = (fov < 87) ? fov + DFOV : fov;
}
void Camera::decreaseFov(){
  fov = (fov > 2) ? fov - DFOV : fov;
}

/** Rotation autour d'un axe quelconque normalisé
 * \param (a, b, c) représente l'axe de rotation, qui est supposé normalisé
 * \param (ang représente l'angle de rotation
 * \param v représente le vecteur sur lequel appliquer la rotation
 */
void Camera::rotate(float a, float b, float c, float ang, float v[4]){
  float m[16];

  // initialiser la matrice de rotation autour de l'axe (a,b,c),
  // d'angle ang.
  // La formule de calcul permettant de générer la matrice de rotation
  // est issue de l'équation :
  // R(v) = cos(ang).v +(1-cos(ang))-(A.v).A + sin(ang)(Axv)
  // avec A l'axe de rotation (a, b, c)
  m[0] = a*a + (1-a*a)*cos(ang);
  m[1] = a*b*(1-cos(ang)) - c*sin(ang);
  m[2] = a*c*(1-cos(ang)) + b*sin(ang);
  m[3] = 0;
  m[4] = a*b*(1-cos(ang)) + c*sin(ang);
  m[5] = b*b + (1-b*b)*cos(ang);
  m[6] = b*c*(1-cos(ang)) - a*sin(ang);
  m[7] = 0;
  m[8] = a*c*(1-cos(ang)) - b*sin(ang);
  m[9] =  b*c*(1-cos(ang)) + a*sin(ang);
  m[10] = c*c + (1-c*c)*cos(ang);
  m[11] = 0;
  m[12] = m[13] = m[14] = 0;
  m[15] = 1;

  // application de la rotation
  mulMatVec(m, v);
  
}

void Camera::crossProduct(float* n, float *v1, float *v2) {
  float v1x = v1[0], v1y = v1[1], v1z = v1[2];
  float v2x = v2[0], v2y = v2[1], v2z = v2[2];
  n[0] =(v1y * v2z) - (v1z * v2y);
  n[1] = (v1z * v2x) - (v1x * v2z);
  n[2] = (v1x * v2y) - (v1y * v2x);
}
