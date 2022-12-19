#include <render/matrix4f.h>
#include <string.h>

struct Matrix4f matrix4f_init() {
    struct Matrix4f matrix;
    matrix.m00 = 1.0f;
    matrix.m01 = 0.0f;
    matrix.m02 = 0.0f;
    matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;
    matrix.m11 = 1.0f;
    matrix.m12 = 0.0f;
    matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;
    matrix.m21 = 0.0f;
    matrix.m22 = 1.0f;
    matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;
    matrix.m31 = 0.0f;
    matrix.m32 = 0.0f;
    matrix.m33 = 1.0f;
    return matrix;
}
void matrix4f_orthographic(struct Matrix4f *matrix, float left, float right,
    float bottom, float top, float near, float far) {

    matrix->m00 = 2.0f / (right - left);
    matrix->m11 = 2.0f / (top - bottom);
    matrix->m22 = 2.0f / (far - near);
    matrix->m30 = -(right + left) / (right - left);
    matrix->m31 = -(top + bottom) / (top - bottom);
    matrix->m32 = -(far + near) / (far - near);
}
void matrix4f_place_into_array_with_a_size_of_sixteen_floats(struct Matrix4f
    matrix, float *array) {

    float temp_array[] = {
        matrix.m00,  matrix.m01,  matrix.m02,  matrix.m03,
        matrix.m10,  matrix.m11,  matrix.m12,  matrix.m13,
        matrix.m20,  matrix.m21,  matrix.m22,  matrix.m23,
        matrix.m30,  matrix.m31,  matrix.m32,  matrix.m33,
    };
    memcpy(array, &temp_array, 16 * sizeof(float));
}
