struct Matrix4 {
  GLfloat m[16];
  Matrix4() { zero(); }
  Matrix4(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4) {
    m[0] = c1.x; m[1] = c1.y; m[2] = c1.z; m[3] = c1.w;
    m[4] = c2.x; m[5] = c2.y; m[6] = c2.z; m[7] = c2.w;
    m[8] = c3.x; m[9] = c3.y; m[10] = c3.z; m[11] = c3.w;
    m[12] = c4.x; m[13] = c4.y; m[14] = c4.z; m[15] = c4.w;
  }
  void zero() {
    std::fill(m, m+16, 0);
  }
  void identity() {
    zero();
    m[0] = m[5] = m[10] = m[15] = 1.0f;
  }

  GLfloat operator[](size_t index) const {
    return m[index];
  }

  GLfloat& operator[](size_t index) {
    return m[index];
  }

  const GLfloat* c_ptr() const { return m; }

  Matrix4 operator*(const Matrix4& mat) const {
    Matrix4 result;
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
        for(int k = 0; k < 4; k++) {
          result[j+i*4] += m[k + i*4]*mat[j + k*4];
        }
      }
    }
    return result;
  }

};
