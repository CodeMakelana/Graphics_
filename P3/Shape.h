#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <GL/glew.h>

struct Shape {
    std::vector<float>        vertices;     // interleaved: x,y,z, r,g,b  (6 floats/vertex)
    std::vector<unsigned int> indices;      // triangle indices  → GL_TRIANGLES
    std::vector<unsigned int> wireIndices;  // edge indices      → GL_LINES

    GLuint VAO, VBO, EBO, wireEBO;
    Shape() : VAO(0), VBO(0), EBO(0), wireEBO(0) {}

    void upload();
    void draw(bool wireframe);
};

#endif