
/*
  pbrt source code is Copyright(c) 1998-2016
  Matt Pharr, Greg Humphreys, and Wenzel Jakob.

  This file is part of pbrt.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  - Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_TRANSFORM_H
#define PBRT_CORE_TRANSFORM_H

// core/transform.h*
//#include "pbrt.h"
//#include "stringprint.h"
//#include "geometry.h"
//#include "quaternion.h"

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

class Transform;

struct Vector4 {
  float x, y, z, t;

  Vector4(){
    x=y=z=0.0; t=0.0;
  }

  Vector4(float x, float y, float z, float t=0.0){
    this->x=x; this->y=y; this->z=z; this->t=t;
  }

  void transformer(const Transform &ctm);

  

  friend std::ostream &operator<<(std::ostream &os, const Vector4 &v) {
    os << "("  << v.x << ","  <<  v.y << "," << v.z << "," << v.t << ")";
  }
  
};

struct Vector3f {
  float x, y, z;

  Vector3f(){
    x=y=z=0.0; 
  };
  
  Vector3f(float x, float y, float z){
    this->x = x; this->y = y; this->z = z;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector3f &v) {
    os << "("  << v.x << ","  <<  v.y << "," << v.z <<  ")";
  }

  float Length() const{
    return  std::sqrt(x*x + y*y +z*z);
  }
};

struct Point3f {
  float x, y, z;

  Point3f(){ x=y=z=0; }
  Point3f(float x, float y, float z): x(x), y(y), z(z){}

  /* Point3f operator-(const Point3f &p) const{ */
  /*   return Point3f(x-p.x, y-p.y, z-p.z); */
  /* } */
  
  Vector3f operator-(const Point3f &p) const{
    return Vector3f(x-p.x, y-p.y, z-p.z);
  }

  void transformer(const Transform &t);

  
  friend std::ostream &operator<<(std::ostream &os, const Point3f &v) {
    os << "("  << v.x << ","  <<  v.y << "," << v.z <<  ")";
  } 

};


inline Vector3f Normalize(const Vector3f &v){
  float l=v.Length();
  return Vector3f(v.x/l, v.y/l, v.z/l);
}

inline Vector3f Cross(const Vector3f &v1, const Vector3f &v2) {
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3f((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
		    (v1x * v2y) - (v1y * v2x));
}

// Matrix4x4 Declarations
struct Matrix4x4 {
  // Matrix4x4 Public Methods
  Matrix4x4() {
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
    m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
      m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
  }
  Matrix4x4(float mat[4][4]);
  Matrix4x4(float t00, float t01, float t02, float t03, float t10, float t11,
	    float t12, float t13, float t20, float t21, float t22, float t23,
	    float t30, float t31, float t32, float t33);
  
  bool operator==(const Matrix4x4 &m2) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	if (m[i][j] != m2.m[i][j]) return false;
    return true;
  }
  bool operator!=(const Matrix4x4 &m2) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	if (m[i][j] != m2.m[i][j]) return true;
    return false;
  }

  void setIdentity(){
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
    m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
      m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
  }
  
  friend Matrix4x4 Transpose(const Matrix4x4 &);

  static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	r.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
	  m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
    return r;
  }
  
  friend Matrix4x4 Inverse(const Matrix4x4 &);

  friend std::ostream &operator<<(std::ostream &os, const Matrix4x4 &m) {
    os << "[";
    for(int i=0; i<4; i++){
      os << "[" <<  m.m[i][0] << ","  <<  m.m[i][1];
      os << "," <<  m.m[i][2] << ","  <<  m.m[i][3] << "]" << endl;
    }
    os << "]";

    // clang-format off
    /* os << StringPrintf("[ [ %f, %f, %f, %f ] " */
    /*                    "[ %f, %f, %f, %f ] " */
    /*                    "[ %f, %f, %f, %f ] " */
    /*                    "[ %f, %f, %f, %f ] ]", */
    /*                    m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3], */
    /*                    m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3], */
    /*                    m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3], */
    /*                    m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]); */
    // clang-format on
    return os;
  }

  float m[4][4];
};

// Transform Declarations
class Transform {
 private:
  // Transform Private Data
  Matrix4x4 m, mInv;
  //    friend class AnimatedTransform;
  //    friend struct Quaternion;
 public:
  // Transform Public Methods
  Transform() {}
  Transform(const float mat[4][4]) {
    m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0],
		  mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1],
		  mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2],
		  mat[3][3]);
    mInv = Inverse(m);
  }
 Transform(const Matrix4x4 &m) : m(m), mInv(Inverse(m)) {}
 Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) : m(m), mInv(mInv) {}

  friend Transform Inverse(const Transform &t) {
    return Transform(t.mInv, t.m);
  }
    
  friend Transform Transpose(const Transform &t) {
    return Transform(Transpose(t.m), Transpose(t.mInv));
  }
    
  bool operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
  }
    
  bool operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
  }
    
  bool operator<(const Transform &t2) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j) {
	if (m.m[i][j] < t2.m.m[i][j]) return true;
	if (m.m[i][j] > t2.m.m[i][j]) return false;
      }
    return false;
  }
    
  bool IsIdentity() const {
    return (m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f &&
	    m.m[0][3] == 0.f && m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
	    m.m[1][2] == 0.f && m.m[1][3] == 0.f && m.m[2][0] == 0.f &&
	    m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
	    m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f &&
	    m.m[3][3] == 1.f);
  }

  void SetIdentity(){
    m.setIdentity();
    mInv.setIdentity();
  }
    
  const Matrix4x4 &GetMatrix() const { return m; }
    
  const Matrix4x4 &GetInverseMatrix() const { return mInv; }
    


  Transform operator*(const Transform &t2) const;
  Vector4 operator*(const Vector4 &v) const;

  bool SwapsHandedness() const;
    


  friend std::ostream &operator<<(std::ostream &os, const Transform &t) {
    os << "t=" << t.m << ", inv=" << t.mInv;
    return os;
  }


};

Transform Translate(float dx, float dy, float dz);
Transform Scale(float x, float y, float z);
Transform RotateX(float theta);
Transform RotateY(float theta);
Transform RotateZ(float theta);
Transform Rotate(float theta, float Ax, float Ay, float Az);
Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);
Transform Orthographic(float znear, float zfar);
Transform Perspective(float fov, float znear, float zfar);
bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float *x0,
                          float *x1);





#endif  // PBRT_CORE_TRANSFORM_H
