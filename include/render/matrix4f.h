#ifndef MATRIX4F_INCLUDED
#define MATRIX4f_INCLUDED

struct Matrix4f {
    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
    float m20;
    float m21;
    float m22;
    float m23;
    float m30;
    float m31;
    float m32;
    float m33;
};
struct Matrix4f matrix4f_init();
void matrix4f_orthographic(struct Matrix4f *matrix, float left, float right,
    float bottom, float top, float near, float far);
void matrix4f_place_into_array_with_a_size_of_sixteen_floats(struct Matrix4f
    matrix, float *array);

#endif