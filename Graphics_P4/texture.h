#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <cmath>

GLuint createColourTexture(int width, int height);
GLuint createDisplacementTexture(int width, int height);
GLuint createAlphaTexture(int width, int height);
void bindTexture(GLuint textureID, int unit, GLuint shaderProgram, const char* uniformName);

#endif