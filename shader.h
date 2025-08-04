#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vectors.h"
typedef struct {
	unsigned int ID;
}Shader;

static char* getShaderCode(const char* path);
Shader shader_create(const char* vertexPath, const char* fragmentPath);
void shader_deleteProg(Shader shader);
void shader_use(Shader shader);
void shader_setBool(Shader shader, const char* name, bool value);
void shader_setInt(Shader shader, const char* name, int value);
void shader_setFloat(Shader shader, const char* name, float value);
void shader_setmat4(Shader shader, const char* name, const mat4_t* mat); 
void shader_setVec3(Shader shader, const char* name, float x, float y, float z);
void shader_checkCompileErrors(unsigned int shaderID, const char* type);
