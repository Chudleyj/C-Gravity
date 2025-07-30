#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define CAPCITY 500

typedef enum {
    VECTOR_SUCESS = 0,
    VECTOR_ERROR_EMPTY = -1,
    VECTOR_ERROR_MALLOC = -2,
    VECTOR_ERROR_REALLOC = -3,
    VECTOR_ERROR_INVALID_INDEX = -4
}vector_result_t;

typedef struct {
    float x, y, z;
}vec3_t;

typedef struct {
    double x, y, z;
}vec3d_t;

typedef struct {
    int size;
    int capacity;
    float *data;
}GLvec3_t;

vector_result_t GLvec3_init(GLvec3_t* vec);
vector_result_t GLvec3_free(GLvec3_t* vec);
static vector_result_t GLvec3_expand(GLvec3_t* vec);
vector_result_t GLvec3_push_point(GLvec3_t* vec, vec3_t point);
vector_result_t GLvec3_push(GLvec3_t* vec, int x, int y, int z);
vector_result_t GLvec3_pop_point(GLvec3_t* vec, vec3_t* point);
vector_result_t GLvec3_pop(GLvec3_t* vec, int* x, int* y, int* z);
vector_result_t GLvec3_get_point(GLvec3_t* vec, int index, vec3_t* point);
vector_result_t GLvec3_get(GLvec3_t* vec, int index, int* x, int* y, int* z);
vec3_t sphericalToCartesian(float rho, float theta, float phi);

vector_result_t vec3_normalize(vec3_t* vec);
vector_result_t vec3_add(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec);
vector_result_t vec3_subtract(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec);
vector_result_t vec3_cross_product(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec);


typedef struct {
    float row[4]; 
}vec4_t;

typedef struct {
    vec4_t column[4]; 
}mat4_t;

mat4_t mat4_lookAt(vec3_t eye, vec3_t center, vec3_t up);
mat4_t mat4_multilply(mat4_t A, mat4_t B);
void mat4_perspective(mat4_t* result, float fov_radians, float aspect, float near_plane, float far_plane);
void mat4_identity(mat4_t* result);
mat4_t mat4_scale(const mat4_t mat, const vec3_t vec);
mat4_t mat4_rotate(const mat4_t mat, const float angle, const vec3_t vec);
mat4_t mat4_translate(const mat4_t mat, const vec3_t vec);