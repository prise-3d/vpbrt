#include "Mesh.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Mesh::Mesh(){
  bb.min.x = bb.min.y = bb.min.z = 1.e100;
  bb.max.x = bb.max.y = bb.max.z = -1.e100;
  mat = nullptr;
}

Mesh::~Mesh(){
  sommets.clear();
  faces.clear();
}

void  Mesh::add(const Point3f &s){
  sommets.push_back(s);
  updateBB(s);
}

void Mesh::add(const Face &f){
  faces.push_back(f);
}

GLmaterial Mesh::getGLmaterial(){
  if(!mat) return GLmaterial();

  return mat->getGLmaterial();
}


void Mesh::updateBB(const Point3f &s){
  if(s.x<bb.min.x) bb.min.x=s.x;
  if(s.x>bb.max.x) bb.max.x=s.x;
  if(s.y<bb.min.y) bb.min.y=s.y;
  if(s.y>bb.max.y) bb.max.y=s.y;
  if(s.z<bb.min.z) bb.min.z=s.z;
  if(s.z>bb.max.z) bb.max.z=s.z;
}

BoundingBox Mesh::getBB(){
  return bb;
}

ostream& operator<<(ostream &out, const Mesh &m){
  out << "----------" << endl;
  out << "mesh : ";
  out << "BB = (" << m.bb.min.x << "," << m.bb.min.y << "," << m.bb.min.z << ")";
  out << " - (" << m.bb.max.x << "," << m.bb.max.y << "," << m.bb.max.z << ")";
  out << endl;
  out << m.sommets.size() << " " << m.faces.size() << endl;

  return out;
}


void Mesh::draw(){
  
  GLmaterial mat = getGLmaterial();

  glColor4f(mat.kd.r, mat.kd.g, mat.kd.b, mat.kd.a);
  
  for(unsigned int i=0; i<faces.size(); i++){
    glBegin(GL_TRIANGLES);
    for(unsigned int s=0; s<3; s++){
      Point3f som=sommets[faces[i].isom[s]];
      glVertex3f(som.x, som.y, som.z);
    }
    glEnd();

  }

}


void Mesh::draw(const Transform &t){
  GLmaterial mat = getGLmaterial();

  glColor4f(mat.kd.r, mat.kd.g, mat.kd.b, mat.kd.a);
  
  for(unsigned int i=0; i<faces.size(); i++){
    glBegin(GL_LINE_LOOP);
    for(unsigned int s=0; s<3; s++){
      Point3f som=sommets[faces[i].isom[s]];
      Vector4 som4={som.x, som.y, som.z, 1.0};
      som4 = t*som4;
      glVertex3f(som4.x, som4.y, som4.z);
      // if(i==0) cout << "(" << som.x << "," << som.y << "," << som.z << ")" << endl;
    }
    glEnd();

  }

}

void Mesh::drawBB(){
  // face inférieure
  glBegin(GL_LINE_LOOP);
  glVertex3f(bb.min.x, bb.min.y, bb.min.z);
  glVertex3f(bb.min.x, bb.min.y, bb.max.z);
  glVertex3f(bb.max.x, bb.min.y, bb.max.z);
  glVertex3f(bb.max.x, bb.min.y, bb.min.z);
  glEnd();


  // face supérieure
  glBegin(GL_LINE_LOOP);
  glVertex3f(bb.min.x, bb.max.y, bb.min.z);
  glVertex3f(bb.min.x, bb.max.y, bb.max.z);
  glVertex3f(bb.max.x, bb.max.y, bb.max.z);
  glVertex3f(bb.max.x, bb.max.y, bb.min.z);
  glEnd();

  // arêtes verticales
  glBegin(GL_LINES);
  glVertex3f(bb.min.x, bb.min.y, bb.min.z);
  glVertex3f(bb.min.x, bb.max.y, bb.min.z);
  glVertex3f(bb.min.x, bb.min.y, bb.max.z);
  glVertex3f(bb.min.x, bb.max.y, bb.max.z);
  glVertex3f(bb.max.x, bb.min.y, bb.min.z);
  glVertex3f(bb.max.x, bb.max.y, bb.min.z);
  glVertex3f(bb.max.x, bb.min.y, bb.max.z);
  glVertex3f(bb.max.x, bb.max.y, bb.max.z);
  glEnd();
}


void Mesh::drawBB(const Transform &t){
  
  Vector4 min={bb.min.x, bb.min.y, bb.min.z, 1.0};
  Vector4 max={bb.max.x, bb.max.y, bb.max.z, 1.0};
  min = t*min;
  max = t*max;
  
  // face inférieure
  glBegin(GL_LINE_LOOP);
  glVertex3f(min.x, min.y, min.z);
  glVertex3f(min.x, min.y, max.z);
  glVertex3f(max.x, min.y, max.z);
  glVertex3f(max.x, min.y, min.z);
  glEnd();


  // face supérieure
  glBegin(GL_LINE_LOOP);
  glVertex3f(min.x, max.y, min.z);
  glVertex3f(min.x, max.y, max.z);
  glVertex3f(max.x, max.y, max.z);
  glVertex3f(max.x, max.y, min.z);
  glEnd();

  // arêtes verticales
  glBegin(GL_LINES);
  glVertex3f(min.x, min.y, min.z);
  glVertex3f(min.x, max.y, min.z);
  glVertex3f(min.x, min.y, max.z);
  glVertex3f(min.x, max.y, max.z);
  glVertex3f(max.x, min.y, min.z);
  glVertex3f(max.x, max.y, min.z);
  glVertex3f(max.x, min.y, max.z);
  glVertex3f(max.x, max.y, max.z);
  glEnd();
}
