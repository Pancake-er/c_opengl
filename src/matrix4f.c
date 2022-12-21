#include <render/matrix4f.h>
#include <string.h>
#include <stdio.h>

void matrix4f_init_array16f(float *matrix_array) {
    // This matrix is in column major order.

    float temp_array[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    memcpy(matrix_array, &temp_array, 16 * sizeof(float));
}
void matrix4f_orthographic(float *matrix, float left, float right, float bottom,
    float top, float near, float far) {
    
    matrix[0] = 2.0f / (right - left);
    matrix[5] = 2.0f / (top - bottom);
    matrix[10] = 2.0f / (far - near);
    matrix[12] = -(right + left) / (right - left);
    matrix[13] = -(top + bottom) / (top - bottom);
    matrix[14] = -(far + near) / (far - near);
}
void matrix4f_scale(float *matrix, float x, float y, float z) {
    matrix[0] *= x;
    matrix[5] *= y;
    matrix[10] *= z;
}
void matrix4f_set_scale(float *matrix, float x, float y, float z) {
    matrix[0] = x;
    matrix[5] = y;
    matrix[10] = z;
}
void matrix4f_translate(float *matrix, float x, float y, float z) {
    matrix[12] += x;
    matrix[13] += y;
    matrix[14] += z;
}
void matrix4f_set_position(float *matrix, float x, float y, float z) {
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}