#include "vectors.h"

vector_result_t GLvec3_init(GLvec3_t *vec) {
    if (!vec) return VECTOR_ERROR_EMPTY;

    vec->size = 0; 
    vec->capacity = CAPCITY; 
    vec->data = malloc(CAPCITY * sizeof(float)); 

    if (!vec->data) {
        return VECTOR_ERROR_MALLOC;
    }

    return VECTOR_SUCESS;
}

vector_result_t GLvec3_free(GLvec3_t* vec) {
    if (!vec->data || !vec) return VECTOR_ERROR_EMPTY;
    free(vec->data);
    return VECTOR_SUCESS;
}

void vector_generic_error_handle(vector_result_t err, const char* msg, vector_error_level_t errLevel){
    switch (err) {
        case VECTOR_ERROR_EMPTY: 
            printf("VECTOR ERROR: VECTOR_ERROR_EMPTY\n %s", msg);
            break;
        case VECTOR_ERROR_MALLOC: 
            printf("VECTOR ERROR: VECTOR_ERROR_MALLOC\n %s", msg);
            break;
        case VECTOR_ERROR_REALLOC: 
            printf("VECTOR ERROR: VECTOR_ERROR_REALLOC\n %s", msg);
            break;
        case VECTOR_ERROR_INVALID_INDEX: 
            printf("VECTOR ERROR: VECTOR_ERROR_INVALID_INDEX\n %s", msg);
            break;
        case VECTOR_ERROR_DIV_ZERO: 
            printf("VECTOR ERROR: VECTOR_ERROR_DIV_ZERO\n %s", msg);
            break;
        default: break; 
    }

    if (errLevel == CRITICAL) {
        exit(EXIT_FAILURE); 
    }
}

static vector_result_t GLvec3_expand(GLvec3_t *vec) {
    if (!vec) return VECTOR_ERROR_EMPTY;
    float *tempData = NULL, *tempy = NULL, *tempz=NULL;
    int newCapacity = vec->capacity * 2; 
    tempData = realloc(vec->data, sizeof(float) * newCapacity);
    if (!tempData){
        return VECTOR_ERROR_REALLOC;
    }
    vec->data = tempData;
    vec->capacity = newCapacity;

    return VECTOR_SUCESS;
}

vector_result_t GLvec3_push_point(GLvec3_t* vec, vec3_t point) {
    vector_result_t ret = VECTOR_SUCESS;

    if (vec->size == vec->capacity) {
        ret = GLvec3_expand(vec);
        if (ret != VECTOR_SUCESS) {
            return ret;
        }
    }
    vec->data[vec->size] = point.x;
    vec->size++;

    if (vec->size == vec->capacity) {
        ret = GLvec3_expand(vec);
        if (ret != VECTOR_SUCESS) {
            return ret;
        }
    }
    vec->data[vec->size]= point.y;
    vec->size++;

    if (vec->size == vec->capacity) {
        ret = GLvec3_expand(vec);
        if (ret != VECTOR_SUCESS) {
            return ret;
        }
    }
    vec->data[vec->size] = point.z;
    vec->size++;

    return ret;
}

vector_result_t GLvec3_push(GLvec3_t *vec, int data) {
    vector_result_t ret = VECTOR_SUCESS;
    if (vec->size == vec->capacity) {
        ret = GLvec3_expand(vec);
        if(ret != VECTOR_SUCESS) {
            return ret;
        }
    }
    vec->data[vec->size] = data;

    vec->size++; 
    return ret;
}

vector_result_t GLvec3_pop_point(GLvec3_t *vec, vec3_t *point){
    if(vec->size == 0){
        return VECTOR_ERROR_EMPTY; 
    }
    point->x = vec->data[vec->size - 1]; 
    vec->size--;
    if (vec->size == 0) {
        return VECTOR_ERROR_EMPTY;
    }
    point->y = vec->data[vec->size - 1];
    vec->size--;
    if (vec->size == 0) {
        return VECTOR_ERROR_EMPTY;
    }
    point->z = vec->data[vec->size - 1];
    vec->size--; 
   
    return VECTOR_SUCESS;
}

vector_result_t GLvec3_pop(GLvec3_t* vec, int *data) {
    if (vec->size == 0) {
        return VECTOR_ERROR_EMPTY;
    }
    *data = vec->data[vec->size - 1];
    vec->size--;

    return VECTOR_SUCESS;
}


vector_result_t GLvec3_get_point(GLvec3_t* vec, int index, vec3_t *point) {
    if (vec->size == 0) {
        return VECTOR_ERROR_EMPTY;
    }

    if (index >= vec->size || index < 0) {
        return VECTOR_ERROR_INVALID_INDEX;
    }

    point->x = vec->data[index]; 
    if (index+1 >= vec->size) {
        return VECTOR_ERROR_INVALID_INDEX;
    }
    point->y = vec->data[index+1];
    if (index + 2 >= vec->size) {
        return VECTOR_ERROR_INVALID_INDEX;
    }
    point->z = vec->data[index+2];

    return VECTOR_SUCESS;
}

vector_result_t GLvec3_get(GLvec3_t* vec, int index, int *data) {
    if (vec->size == 0) {
        return VECTOR_ERROR_EMPTY;
    }

    if (index >= vec->size || index < 0) {
        return VECTOR_ERROR_INVALID_INDEX;
    }

    *data = vec->data[index];

    return VECTOR_SUCESS;
}

vector_result_t vec3_normalize(vec3_t* vec) {
    if (!vec) return VECTOR_ERROR_EMPTY;
    float magnitude = sqrtf(powf(vec->x, 2) + powf(vec->y, 2) + powf(vec->z, 2));
    if (magnitude == 0.0f) return VECTOR_ERROR_DIV_ZERO;
    vec->x /= magnitude;
    vec->y /= magnitude;
    vec->z /= magnitude;
    return VECTOR_SUCESS;
}

vector_result_t vec3_add(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec) {
    if (!tgtVec) return VECTOR_ERROR_EMPTY;

    tgtVec->x = srcVec1.x + srcVec2.x;
    tgtVec->y = srcVec1.y + srcVec2.y;
    tgtVec->z = srcVec1.z + srcVec2.z;

    return VECTOR_SUCESS;
}


vector_result_t vec3d_add(const vec3d_t srcVec1, const vec3d_t srcVec2, vec3d_t* tgtVec) {
    if (!tgtVec) return VECTOR_ERROR_EMPTY;

    tgtVec->x = srcVec1.x + srcVec2.x;
    tgtVec->y = srcVec1.y + srcVec2.y;
    tgtVec->z = srcVec1.z + srcVec2.z;

    return VECTOR_SUCESS;
}

vector_result_t vec3d_scale(vec3d_t* vec, double scale) {
    if (!vec) return VECTOR_ERROR_EMPTY;

    vec->x *= scale;
    vec->y *= scale;
    vec->z *= scale;
    return VECTOR_SUCESS;
    
}

vector_result_t vec3_subtract(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec) {
    if (!tgtVec) return VECTOR_ERROR_EMPTY;

    tgtVec->x = srcVec1.x - srcVec2.x;
    tgtVec->y = srcVec1.y - srcVec2.y;
    tgtVec->z = srcVec1.z - srcVec2.z;

    return VECTOR_SUCESS;
}


vector_result_t vec3d_subtract(const vec3d_t srcVec1, const vec3d_t srcVec2, vec3d_t* tgtVec) {
    if (!tgtVec) return VECTOR_ERROR_EMPTY;

    tgtVec->x = srcVec1.x - srcVec2.x;
    tgtVec->y = srcVec1.y - srcVec2.y;
    tgtVec->z = srcVec1.z - srcVec2.z;

    return VECTOR_SUCESS;
}

vector_result_t vec3_cross_product(const vec3_t srcVec1, const vec3_t srcVec2, vec3_t* tgtVec) {
    tgtVec->x = srcVec1.y * srcVec2.z - srcVec1.z * srcVec2.y;
    tgtVec->y = srcVec1.z * srcVec2.x - srcVec1.x * srcVec2.z;
    tgtVec->z = srcVec1.x * srcVec2.y - srcVec1.y * srcVec2.x;
}

vec3_t vec3_sphericalToCartesian(float rho, float theta, float phi) {
    vec3_t cartesian = { 0 };
    cartesian.x = rho * sinf(theta) * cosf(phi);
    cartesian.y = rho * sinf(theta) * sinf(phi);
    cartesian.z = rho * cosf(theta);
    return cartesian;
}




mat4_t mat4_lookAt(vec3_t position, vec3_t target, vec3_t up){
    vec3_t z; 
    vec3_subtract(position, target, &z); 
    vec3_normalize(&z); 

    vec3_t x; 
    vec3_normalize(&up); 
    vec3_cross_product(up, z, &x); 
    vec3_normalize(&x); 

    vec3_t y; 
    vec3_cross_product(z, x, &y); 

    mat4_t translation; 
    mat4_identity(&translation);
    translation.column[3].row[0] = position.x * -1;
    translation.column[3].row[1] = position.y * -1;
    translation.column[3].row[2] = position.z * -1;

    mat4_t rotation; 
    mat4_identity(&rotation);
    rotation.column[0].row[0] = x.x; // First column, first row
    rotation.column[1].row[0] = x.y;
    rotation.column[2].row[0] = x.z;
    rotation.column[0].row[1] = y.x; // First column, second row
    rotation.column[1].row[1] = y.y;
    rotation.column[2].row[1] = y.z;
    rotation.column[0].row[2] = z.x; // First column, third row
    rotation.column[1].row[2] = z.y;
    rotation.column[2].row[2] = z.z;

    mat4_t lookMat = mat4_multilply(rotation, translation);
    return lookMat;
}
/*
C[row][col] = A[row][0] * B[0][col] +
              A[row][1] * B[1][col] +
              A[row][2] * B[2][col] +
              A[row][3] * B[3][col];
*/
mat4_t mat4_multilply(mat4_t A, mat4_t B) {
    mat4_t C = { 0 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            C.column[i].row[j] = 0; 
            for (int k = 0; k < 4; k++) {
                C.column[i].row[j] += A.column[i].row[k] * B.column[k].row[j];
            }
        }
    }
    return C; 
}


void mat4_perspective(mat4_t* result, float fov_radians, float aspect, float near_plane, float far_plane) {
    // Clear the matrix first
    memset(result, 0, sizeof(mat4_t));

    float tan_half_fov = tanf(fov_radians / 2.0f);

    // OpenGL perspective matrix:
    // [ 1/(aspect*tan(fov/2))    0              0                           0           ]
    // [ 0                       1/tan(fov/2)    0                           0           ]
    // [ 0                       0              -(far+near)/(far-near)     -2*far*near/(far-near) ]
    // [ 0                       0              -1                          0           ]

    result->column[0].row[0] = 1.0f / (aspect * tan_half_fov);
    result->column[1].row[1] = 1.0f / tan_half_fov;
    result->column[2].row[2] = -(far_plane + near_plane) / (far_plane - near_plane);
    result->column[2].row[3] = -1.0f;
    result->column[3].row[2] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
}

void mat4_identity(mat4_t* result) {
    memset(result, 0, sizeof(mat4_t));
    result->column[0].row[0] = 1.0f;
    result->column[1].row[1] = 1.0f;
    result->column[2].row[2] = 1.0f;
    result->column[3].row[3] = 1.0f;
}

mat4_t mat4_dtranslate(const mat4_t mat, const vec3d_t vec) {
    mat4_t result = mat;

    result.column[3].row[0] = (float)mat.column[3].row[0] + vec.x;
    result.column[3].row[1] = (float)mat.column[3].row[1] + vec.y;
    result.column[3].row[2] = (float)mat.column[3].row[2] + vec.z;
    result.column[3].row[3] = 1.0f;

    return result;
}


mat4_t mat4_translate(const mat4_t mat, const vec3_t vec) {
    mat4_t result = mat;

    result.column[3].row[0] = mat.column[3].row[0] + vec.x;
    result.column[3].row[1] = mat.column[3].row[1] + vec.y;
    result.column[3].row[2] = mat.column[3].row[2] + vec.z;
    result.column[3].row[3] = 1.0f;

    return result;
}


mat4_t mat4_scale(const mat4_t mat, const vec3_t vec) {
    mat4_t result = mat;

    // Scale entire columns, not just diagonal
    for (int i = 0; i < 4; i++) {
        result.column[0].row[i] *= vec.x;
        result.column[1].row[i] *= vec.y;
        result.column[2].row[i] *= vec.z;
    }

    result.column[3].row[3] = 1.0f;

    return result; 
}

mat4_t mat4_rotate(const mat4_t mat, const float angle, const vec3_t vec) {
    const float a = angle; 
    const float c = cosf(a); 
    const float s = sinf(a); 

    vec3_t axis = vec;
    vec3_normalize(&axis);

    vec3_t temp = { 0 };
    temp.x = (1.0f - c) * axis.x;
    temp.y = (1.0f - c) * axis.y;
    temp.z = (1.0f - c) * axis.z;

    mat4_t rotate = { 0 };

    rotate.column[0].row[0] = c + temp.x * axis.x; 
    rotate.column[0].row[1] = temp.x * axis.y + s * axis.z;
    rotate.column[0].row[2] = temp.x * axis.z - s * axis.y;

    rotate.column[1].row[0] = temp.y * axis.x - s * axis.z;
    rotate.column[1].row[1] = c + temp.y * axis.y; 
    rotate.column[1].row[2] = temp.y * axis.z + s * axis.x;

    rotate.column[2].row[0] = temp.z * axis.x + s * axis.y;
    rotate.column[2].row[1] = temp.z * axis.y - s * axis.x;
    rotate.column[2].row[2] = c + temp.z * axis.z; 

    mat4_t result = { 0 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result.column[i].row[j] += mat.column[k].row[j] * rotate.column[i].row[k];
            }
        }
    }

    result.column[3] = mat.column[3]; 
    return result; 
}





