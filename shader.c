#include "shader.h"

char* getShaderCode(const char* path) {
    FILE* fp;
    errno_t fileErr;

    fileErr = fopen_s(&fp, path, "r");
    if (fileErr != 0) {
        char errBuf[256];
        strerror_s(errBuf, sizeof(errBuf), fileErr);
        printf("Failed opening shader file: %s : %s!\n", path, errBuf);
        exit(EXIT_FAILURE);
    }

    size_t buffSize = 1024;
    size_t len = 0;
    char* code = malloc(buffSize * sizeof(char));
    if (!code) {
        printf("Shader code malloc failure\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    code[0] = '\0';
    errno_t strcatErr;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t lineLen = strlen(line);
        if (len + lineLen + 1 > buffSize) {
            buffSize *= 2;
            char* newCode = realloc(code, buffSize * sizeof(char));
            if (newCode == NULL) {
                printf("Shader code realloc failure\n");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            code = newCode;
        }
        strcatErr = strcat_s(code, buffSize, line);
        if (strcatErr != 0) {
            printf("Strcat error: %d\n", strcatErr);
            exit(EXIT_FAILURE);
        }
        len += lineLen;
    }
    fclose(fp);

    return code;
}

Shader shader_create(const char* vertexPath, const char* fragmentPath) {
    const char* vertexCode = getShaderCode(vertexPath);
    const char* fragmentCode = getShaderCode(fragmentPath);

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    shader_checkCompileErrors(vertex, "VERTEX");

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    shader_checkCompileErrors(fragment, "FRAGMENT");

    Shader newShader;
    newShader.ID = glCreateProgram();
    glAttachShader(newShader.ID, vertex);
    glAttachShader(newShader.ID, fragment);
    glLinkProgram(newShader.ID);
    shader_checkCompileErrors(newShader.ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free((void*)vertexCode);
    free((void*)fragmentCode);

    return newShader;
}

void shader_use(Shader shader) {
    glUseProgram(shader.ID);
}

void shader_deleteProg(Shader shader) {
    glDeleteProgram(shader.ID);
}

void shader_setBool(Shader shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader.ID, name), (int)value);
}

void shader_setInt(Shader shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void shader_setFloat(Shader shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader.ID, name), value);
}


void shader_setmat4(Shader shader, const char* name, const mat4_t* mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, &(mat->column[0].row[0]));
}

void  shader_setVec3(Shader shader, const char* name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(shader.ID, name), x, y, z);
}

void shader_checkCompileErrors(unsigned int shaderID, const char* type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR: %s\n %s", type, infoLog);
            exit(EXIT_FAILURE);

        }
        else
        {
            glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
                printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n %s", type, infoLog);
                exit(EXIT_FAILURE);
            }
        }
    }
}