#include "Cylindre.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Cylindre::Cylindre(){
  rayon = 1.0;
  zmin = -1.0;
  zmax = 1.0;
  mat = nullptr;
  deb = Point3f(0, 0, zmin);
  fin = Point3f(0, 0, zmax);
}

Cylindre::Cylindre(float r, float minz, float maxz){
  rayon = r;
  zmin = minz;
  zmax = maxz;
  mat = nullptr;
  deb = Point3f(0, 0, zmin);
  fin = Point3f(0, 0, zmax);
}

Cylindre::~Cylindre(){
  if(mat) delete mat;
}

BoundingBox Cylindre::getBB(){
  BoundingBox bb = {deb, fin};
  return bb;
}

void Cylindre::draw(){
  GLmaterial mat = getGLmaterial();

  glColor4f(mat.kd.r, mat.kd.g, mat.kd.b, mat.kd.a);
  glColor4f(0.0, 0.0, 1.0, 1.0);

  glBegin(GL_LINES);
  glVertex3f(deb.x, deb.y, deb.z);
  glVertex3f(fin.x, fin.y, fin.z);
  glEnd();

}


void Cylindre::draw(const Transform &t){
  GLmaterial mat = getGLmaterial();

  glColor4f(mat.kd.r, mat.kd.g, mat.kd.b, mat.kd.a);
  glBegin(GL_LINES);
  Vector4 min={deb.x, deb.y, deb.z, 1.0};
  Vector4 max={deb.x, deb.y, deb.z, 1.0};
  min = t*min; max = t*max;
  glVertex3f(min.x, min.y, min.z);
  glVertex3f(max.x, max.y, max.z);
  glEnd();

}

void Cylindre::drawBB(){
  draw();
}

void Cylindre::drawBB(const Transform &t){
  draw(t);
}


GLmaterial Cylindre::getGLmaterial(){
  if(!mat) return GLmaterial();

  return mat->getGLmaterial();
}

void Cylindre::transform(const Transform &t){
  static bool prems=true;

  if(prems) std::cout << t << std::endl;
  Vector4 min={deb.x, deb.y, deb.z, 1.0};
  Vector4 max={fin.x, fin.y, fin.z, 1.0};
  if(prems)
    std::cout << "t: " << min << " " << max;
  min = t*min; max = t*max;
  deb.x = min.x; deb.y = min.y; deb.z = min.z;
  fin.x = max.x; fin.y = max.y; fin.z = max.z;
  if(prems) std::cout << " / " << min << " " << max << std::endl;
  prems=false;

}

ostream& operator<<(ostream &out, const Cylindre &c){
  out << "----------" << endl;
  out << "Cylindre : ";

  out << c.deb << " " << c.fin << " " << c.zmin << " " << c.zmax << std::endl;

  return out;
}
