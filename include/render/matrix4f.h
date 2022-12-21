#ifndef MATRIX4F_INCLUDED
#define MATRIX4f_INCLUDED

void matrix4f_init_array16f(float *matrix);
void matrix4f_orthographic(float *matrix, float left, float right,
    float bottom, float top, float near, float far);
void matrix4f_scale(float *matrix, float x, float y, float z);
void matrix4f_translate(float *matrix, float x, float y, float z);
void matrix4f_set_position(float *matrix, float x, float y, float z);

#endif