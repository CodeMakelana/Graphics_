#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>

struct SphereVertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct SphereMesh {
    SphereVertex* vertices;
    unsigned int* indices;
    unsigned int* wireframeIndices;
    int vertexCount;
    int indexCount;
    int wireframeIndexCount;
    GLuint VAO, VBO, EBO, wireEBO;
    int currentStacks;
    int currentSectors;
    float radius;
};

SphereMesh generateSphere(int stacks, int sectors, float radius);
void uploadSphereToGPU(SphereMesh& mesh);
void updateSphereResolution(SphereMesh& mesh, int newStacks, int newSectors, float radius);
void drawSphere(const SphereMesh& mesh, bool wireframe);
void cleanupSphere(SphereMesh& mesh);

#endif