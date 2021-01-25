#ifndef _MATRIX_4X4_HPP
#define _MATRIX_4X4_HPP

class Matrix4x4 {
private :
  float m[4][4];
public :
  Matrix4x4();
  Matrix4x4(Float mat[4][4]);
  Matrix4x4(Float t00, Float t01, Float t02, Float t03, Float t10, Float t11,
	    Float t12, Float t13, Float t20, Float t21, Float t22, Float t23,
	    Float t30, Float t31, Float t32, Float t33);
  bool operator==(const Matrix4x4 &m2) const;
  
  bool operator!=(const Matrix4x4 &m2) const;

  
  

  static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	r.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
	  m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
    return r;
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix4x4 &m) {
    // clang-format off
    os << StringPrintf("[ [ %f, %f, %f, %f ] "
		       "[ %f, %f, %f, %f ] "
		       "[ %f, %f, %f, %f ] "
		       "[ %f, %f, %f, %f ] ]",
		       m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		       m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		       m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		       m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
    // clang-format on
    return os;
  }

    friend Matrix4x4 Transpose(const Matrix4x4 &);
    friend Matrix4x4 Inverse(const Matrix4x4 &);


};

#endif
