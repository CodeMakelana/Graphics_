#include "sphere.h"
#include <cmath>

const float PI = 3.14159265358979323846f;

SphereMesh generateSphere(int stacks, int sectors, float radius) {
    SphereMesh mesh;
    SphereVertex verts;
    
    //calc array sizes;
    mesh.vertexCount = (stacks + 1) * (sectors + 1);
    mesh.indexCount = stacks * sectors * 6;
    mesh.wireframeIndexCount = stacks * sectors * 4;
    mesh.radius = radius;

    //allocate the arrays
    mesh.vertices = new SphereVertex [mesh.vertexCount];
    mesh.indices = new unsigned int [mesh.indexCount];
    mesh.wireframeIndices = new unsigned int [mesh.wireframeIndexCount];

    //vertex loop
    for (int i = 0; i <= stacks; i++) { //each i reps 1 ring for the sphere
        float phi = PI * i / stacks;
        for (int j = 0; j <= sectors; j++) {
            float theta = 2 * PI * ((float)j / sectors);
            verts.x = mesh.radius * sin(phi) * cos(theta);
            verts.y = mesh.radius * cos(phi);
            verts.z = mesh.radius * sin(phi) * sin(theta);

            verts.nx = verts.x / mesh.radius;
            verts.ny = verts.y / mesh.radius;
            verts.nz = verts.z / mesh.radius;

            verts.u = (float)j / sectors;
            verts.v = (float)i / stacks;

            int indexArray = i * (sectors + 1) + j;

            mesh.vertices[indexArray] = verts;
        }
    }

    //triangle index loop
    int idx = 0;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < sectors; j++) {
            unsigned int k1 = i * (sectors + 1) + j;
            unsigned int k2 = (i + 1) * (sectors + 1) + j;

            mesh.indices[idx] = k1;
            mesh.indices[idx + 1] = k2;
            mesh.indices[idx + 2] = k1 + 1;

            mesh.indices[idx + 3] = k1 + 1;
            mesh.indices[idx + 4] = k2;
            mesh.indices[idx + 5] = k2 + 1;

            idx += 6;
        }
    }

    //wirframe index
    int wIdx = 0;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < sectors; j++) {
            unsigned int k1 = i * (sectors + 1) + j;
            unsigned int k2 = (i + 1) * (sectors + 1) + j;
            mesh.wireframeIndices[wIdx] = k1;
            mesh.wireframeIndices[wIdx + 1] = k1 + 1;
            mesh.wireframeIndices[wIdx + 2] = k1;
            mesh.wireframeIndices[wIdx + 3] = k2;
            wIdx += 4;
        }
    }

    mesh.currentStacks = stacks;
    mesh.currentSectors = sectors;
    mesh.radius = radius;

    mesh.VAO = mesh.VBO = mesh.EBO = mesh.wireEBO = 0; // GPU handles will be set in uploadSphereToGPU

    return mesh;
}

void uploadSphereToGPU(SphereMesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    glGenBuffers(1, &mesh.wireEBO);

    glBindVertexArray(mesh.VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(SphereVertex), mesh.vertices, GL_STATIC_DRAW);

    // Upload triangle index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexCount * sizeof(unsigned int), mesh.indices, GL_STATIC_DRAW);

    // Upload wireframe index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.wireEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.wireframeIndexCount * sizeof(unsigned int), mesh.wireframeIndices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, x));
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)0);

    // Normal (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, nx));
    glEnableVertexAttribArray(1);

    // Texcoord (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, u));
    glEnableVertexAttribArray(2);

    //unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void updateSphereResolution(SphereMesh& mesh, int newStacks, int newSectors, float radius) {

    if ((newStacks == mesh.currentStacks) && newSectors == mesh.currentSectors) {
        return;
    }

    //clean up old GPU buffers since we are updating
    cleanupSphere(mesh);

    //regenerate
    mesh = generateSphere(newStacks, newSectors, radius);

    uploadSphereToGPU(mesh);

}

void drawSphere(const SphereMesh& mesh, bool wireframe) {
    //bind the VAO
    glBindVertexArray(mesh.VAO);

    //bind right EBO and issue draw call

    if (wireframe) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    //unbind
    glBindVertexArray(0);
}

void cleanupSphere(SphereMesh& mesh) {
    //delete GPU objs
    glDeleteBuffers(1, &mesh.VBO);
    glDeleteBuffers(1, &mesh.EBO);
    glDeleteBuffers(1, &mesh.wireEBO);
    glDeleteBuffers(1, &mesh.VAO);

    //free the CPU arrays
    delete [] mesh.vertices;
    delete [] mesh.indices;
    delete [] mesh.wireframeIndices;

    mesh.vertices = NULL;
    mesh.indices = NULL;
    mesh.wireframeIndices = NULL;

    mesh.vertexCount = mesh.indexCount = mesh.wireframeIndexCount = 0;

    mesh.VAO = mesh.VBO = mesh.EBO = mesh.wireEBO = 0;

}