
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


// core/transform.cpp*
#include "transform.h"
//#include "interaction.h"
#include <cmath>
using namespace std;


#define Radians(deg) ((M_PI / 180.0) * deg)
#define Error(txt)  cerr << txt


void Point3f::transformer(const Transform &t){
    Vector4 v={x, y, z, 1.0};
    v = t * v;
    x = v.x/v.t; y = v.y/v.t; z = v.z/v.t;
}

void Vector4::transformer(const Transform &ctm)
  { 
    *this = ctm * *this;
    x = x / t;
    y = y / t;
    z = z / t;
    t = 1;
  }

// Matrix4x4 Method Definitions
bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float *x0,
                          float *x1) {
    float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    if (std::abs(det) < 1e-10f) return false;
    *x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
    *x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
    if (std::isnan(*x0) || std::isnan(*x1)) return false;
    return true;
}

Matrix4x4::Matrix4x4(float mat[4][4]) { memcpy(m, mat, 16 * sizeof(float)); }

Matrix4x4::Matrix4x4(float t00, float t01, float t02, float t03, float t10,
                     float t11, float t12, float t13, float t20, float t21,
                     float t22, float t23, float t30, float t31, float t32,
                     float t33) {
    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;
    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;
    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;
    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
}

Matrix4x4 Transpose(const Matrix4x4 &m) {
    return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1],
                     m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2],
                     m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3],
                     m.m[3][3]);
}

Matrix4x4 Inverse(const Matrix4x4 &m) {
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    float minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = float(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        Error("Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.f) Error("Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    // cout << Matrix4x4(minv) << endl;
    return Matrix4x4(minv);
}

// Transform Method Definitions


Transform Translate(float dx, float dy, float dz) {
    Matrix4x4 m(1, 0, 0, dx, 0, 1, 0, dy, 0, 0, 1, dz, 0, 0, 0,
                1);
    Matrix4x4 minv(1, 0, 0, -dx, 0, 1, 0, -dy, 0, 0, 1, -dz, 0,
                   0, 0, 1);
    return Transform(m, minv);
}

Transform Scale(float x, float y, float z) {
    Matrix4x4 m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
    Matrix4x4 minv(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
    return Transform(m, minv);
}

Transform RotateX(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0,
                0, 0, 0, 1);
    return Transform(m, Transpose(m));
}

Transform RotateY(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta, 0, cosTheta, 0,
                0, 0, 0, 1);
    return Transform(m, Transpose(m));
}

Transform RotateZ(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 1);
    return Transform(m, Transpose(m));
}

Transform Rotate(float theta, float Ax, float Ay, float Az) {
  // normalisation de l'axe
  float lg = sqrt(Ax*Ax+Ay*Ay+Az*Az);
  float ax = Ax / lg;
  float ay = Ay / lg;
  float az = Az / lg;
  
  //  Vector3f a = Normalize(axis);
  
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    Matrix4x4 m;
    // Compute rotation of first basis vector
    m.m[0][0] = ax * ax + (1 - ax * ax) * cosTheta;
    m.m[0][1] = ax * ay * (1 - cosTheta) - az * sinTheta;
    m.m[0][2] = ax * az * (1 - cosTheta) + ay * sinTheta;
    m.m[0][3] = 0;

    // Compute rotations of second and third basis vectors
    m.m[1][0] = ax * ay * (1 - cosTheta) + az * sinTheta;
    m.m[1][1] = ay * ay + (1 - ay * ay) * cosTheta;
    m.m[1][2] = ay * az * (1 - cosTheta) - ax * sinTheta;
    m.m[1][3] = 0;

    m.m[2][0] = ax * az * (1 - cosTheta) - ay * sinTheta;
    m.m[2][1] = ay * az * (1 - cosTheta) + ax * sinTheta;
    m.m[2][2] = az * az + (1 - az * az) * cosTheta;
    m.m[2][3] = 0;
    return Transform(m, Transpose(m));
}


Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Matrix4x4 cameraToWorld;
    // Initialize fourth column of viewing matrix
    cameraToWorld.m[0][3] = pos.x;
    cameraToWorld.m[1][3] = pos.y;
    cameraToWorld.m[2][3] = pos.z;
    cameraToWorld.m[3][3] = 1;

    // Initialize first three columns of viewing matrix
    Vector3f dir = Normalize(look - pos);
    if (Cross(Normalize(up), dir).Length() == 0) {
      std::cerr << "\"up\" vector and viewing direction " 
		<< "passed to LookAt are pointing in the same direction.  Using "
		<< "the identity transformation." << std::endl;
      return Transform();
    }
    
    Vector3f left = Normalize(Cross(Normalize(up), dir));
    Vector3f newUp = Cross(dir, left);
    cameraToWorld.m[0][0] = left.x;
    cameraToWorld.m[1][0] = left.y;
    cameraToWorld.m[2][0] = left.z;
    cameraToWorld.m[3][0] = 0.;
    cameraToWorld.m[0][1] = newUp.x;
    cameraToWorld.m[1][1] = newUp.y;
    cameraToWorld.m[2][1] = newUp.z;
    cameraToWorld.m[3][1] = 0.;
    cameraToWorld.m[0][2] = dir.x;
    cameraToWorld.m[1][2] = dir.y;
    cameraToWorld.m[2][2] = dir.z;
    cameraToWorld.m[3][2] = 0.;
    
    return Transform(Inverse(cameraToWorld), cameraToWorld);
}




Transform Transform::operator*(const Transform &t2) const {
    return Transform(Matrix4x4::Mul(m, t2.m), Matrix4x4::Mul(t2.mInv, mInv));
}

Vector4 Transform::operator*(const Vector4 &v) const {
  Vector4 res;

  res.x = m.m[0][0]*v.x +  m.m[0][1]*v.y +  m.m[0][2]*v.z +  m.m[0][3]*v.t;
  res.y = m.m[1][0]*v.x +  m.m[1][1]*v.y +  m.m[1][2]*v.z +  m.m[1][3]*v.t;
  res.z = m.m[2][0]*v.x +  m.m[2][1]*v.y +  m.m[2][2]*v.z +  m.m[2][3]*v.t;
  res.t = m.m[3][0]*v.x +  m.m[3][1]*v.y +  m.m[3][2]*v.z +  m.m[3][3]*v.t;

  return res;

}

bool Transform::SwapsHandedness() const {
    float det = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
                m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
                m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
    return det < 0;
}


Transform Orthographic(float zNear, float zFar) {
    return Scale(1, 1, 1 / (zFar - zNear)) * Translate(0, 0, -zNear);
}

Transform Perspective(float fov, float n, float f) {
    // Perform projective divide for perspective projection
    Matrix4x4 persp(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, f / (f - n), -f * n / (f - n),
                    0, 0, 1, 0);

    // Scale canonical perspective view to specified field of view
    float invTanAng = 1 / std::tan(Radians(fov) / 2);
    return Scale(invTanAng, invTanAng, 1) * Transform(persp);
}



