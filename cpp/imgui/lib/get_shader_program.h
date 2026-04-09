#include <stdio.h>
#include <stdlib.h>

#include "../glad/glad.h"

typedef struct {
    const GLenum type;
    const char *path;
} Shader;

GLuint compile_shader(GLenum shaderType, int shdrSrcLen, const char *shdrSrc[]) {
    // Compile the shader
    unsigned int shdr;
    shdr = glCreateShader(shaderType);
    glShaderSource(shdr, shdrSrcLen, shdrSrc, NULL);
    glCompileShader(shdr);

    // Check shader compilation errors
    int success;
    glGetShaderiv(shdr, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shdr, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation failed\n%s\n", infoLog);
        return 0;
    }

    return shdr;
}

char *read_file(const char *path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        perror(path);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *str = (char *)malloc(fsize + 1);
    if (fread(str, fsize, 1, f) != 1) {
        perror(path);
        exit(1);
    }
    fclose(f);

    str[fsize] = 0;

    return str;
}

GLuint get_shader_program(Shader *shaders, int len) {

    GLuint shaderProgram = glCreateProgram();

    // Compile shaders and attach them to the program
    for (int i = 0; i < len; i++) {
        const int shdrSrcLen = 1;
        const char *shdrSrc[1] = { read_file(shaders[i].path) };
        GLuint shdr = compile_shader(shaders[i].type, shdrSrcLen, shdrSrc);
        free((void *)shdrSrc[0]);
        glAttachShader(shaderProgram, shdr);
        glDeleteShader(shdr);
    }

    // Link the attached shaders
    glLinkProgram(shaderProgram);

    // Check linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking failed\n%s\n", infoLog);
        return 1;
    }

    return shaderProgram;
}