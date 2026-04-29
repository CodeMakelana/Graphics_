#ifndef PLANE_H
#define PLANE_H

#include <GL/glew.h>

struct PlaneVertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct PlaneMesh {
    PlaneVertex* vertices;
    unsigned int* indices;
    unsigned int* wireframeIndices;
    int vertexCount;
    int indexCount;
    int wireframeIndexCount;
    GLuint VAO, VBO, EBO, wireEBO;
    int currentResolution;
    float size;
};

PlaneMesh generatePlane(int gridResolution, float size);
void uploadPlaneToGPU(PlaneMesh& mesh);
void updatePlaneResolution(PlaneMesh& mesh, int newResolution, float size);
void drawPlane(const PlaneMesh& mesh, bool wireframe);
void cleanupPlane(PlaneMesh& mesh);

#endif