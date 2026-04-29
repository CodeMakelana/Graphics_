#include "plane.h"

PlaneMesh generatePlane(int gridResolution, float size) {
    PlaneMesh mesh;

    int N = gridResolution;

    float negReach  = -size / 2;
    float posReach = size / 2;
    float stepSize = size / N;

    mesh.vertexCount = (N + 1) * (N + 1);
    mesh.indexCount = N * N * 6;
    mesh.wireframeIndexCount = N * N * 8;

    //allocate arrays
    mesh.vertices = new PlaneVertex [mesh.vertexCount];
    mesh.indices = new unsigned int [mesh.indexCount];
    mesh.wireframeIndices = new unsigned int [mesh.wireframeIndexCount];

    //double loop over i to N and j to N, computing position,
        //writing normal as (0,1,0), computing UV, and storing into vertices[i * (N + 1) + j]

    for (int i = 0; i <= N; i++) {
        float z = negReach + i * stepSize;
        for (int j = 0; j <= N; j++) {
            float x = negReach + j * stepSize;

            PlaneVertex verts;
            verts.x = x;
            verts.y = 0.0;
            verts.z = z;

            verts.nx = 0;
            verts.ny = 1;
            verts.nz = 0;

            verts.u = (float)j / N;
            verts.v = (float)i / N;

            mesh.vertices[i * (N + 1) + j] = verts;
        }
    }

    //triangle indexing
    int idx = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsigned int topLeft = i * (N+1) + j;
            unsigned int topRight = i * (N+1) + (j+1);
            unsigned int bottomLeft = (i+1) * (N+1) + j;
            unsigned int bottomRight = (i+1) * (N+1) + (j+1);

            //Triangle 1 (top left, bottom left, top right)
            mesh.indices[idx] = topLeft;
            mesh.indices[idx + 1] = bottomLeft;
            mesh.indices[idx + 2] = topRight;

            //Triangle 2 (top right, bottom left, bottom right)
            mesh.indices[idx + 3] = topRight;
            mesh.indices[idx + 4] = bottomLeft;
            mesh.indices[idx + 5] = bottomRight;

            idx += 6;
        }
    }
        //wireframe indexing
    int wIdx = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsigned int k1 = i * (N + 1) + j;
            unsigned int k2 = (i + 1) * (N + 1) + j;
            unsigned int k3 = i * (N + 1) + (j + 1);
            unsigned int k4 = (i + 1) * (N + 1) + (j + 1);

            mesh.wireframeIndices[wIdx] = k1;
            mesh.wireframeIndices[wIdx + 1] = k2;
            mesh.wireframeIndices[wIdx + 2] = k2;
            mesh.wireframeIndices[wIdx + 3] = k4;
            mesh.wireframeIndices[wIdx + 4] = k4;
            mesh.wireframeIndices[wIdx + 5] = k3;
            mesh.wireframeIndices[wIdx + 6] = k3;
            mesh.wireframeIndices[wIdx + 7] = k1;

            wIdx += 8;
        }
    }

    mesh.currentResolution = gridResolution;
    mesh.size = size;

    mesh.VAO = mesh.VBO = mesh.EBO = mesh.wireEBO = 0;

    return mesh;
}

void uploadPlaneToGPU(PlaneMesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    glGenBuffers(1, &mesh.wireEBO);

    glBindVertexArray(mesh.VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(PlaneVertex), mesh.vertices, GL_STATIC_DRAW);

    // Upload triangle index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexCount * sizeof(unsigned int), mesh.indices, GL_STATIC_DRAW);

    // Upload wireframe index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.wireEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.wireframeIndexCount * sizeof(unsigned int), mesh.wireframeIndices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texcoord (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void updatePlaneResolution(PlaneMesh& mesh, int newResolution, float size) {
    if (newResolution == mesh.currentResolution) {
        return;
    }

    cleanupPlane(mesh);

    mesh = generatePlane(newResolution, size);

    uploadPlaneToGPU(mesh);
}

void drawPlane(const PlaneMesh& mesh, bool wireframe) {
    glBindVertexArray(mesh.VAO);

    if (wireframe) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.wireEBO);
        glDrawElements(GL_LINES, mesh.wireframeIndexCount, GL_UNSIGNED_INT, 0);
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void cleanupPlane(PlaneMesh& mesh) {
    if (mesh.VAO != 0) {
        glDeleteVertexArrays(1, &mesh.VAO);
        mesh.VAO = 0;
    }
    if (mesh.VBO != 0) {
        glDeleteBuffers(1, &mesh.VBO);
        mesh.VBO = 0;
    }
    if (mesh.EBO != 0) {
        glDeleteBuffers(1, &mesh.EBO);
        mesh.EBO = 0;
    }
    if (mesh.wireEBO != 0) {
        glDeleteBuffers(1, &mesh.wireEBO);
        mesh.wireEBO = 0;
    }

    delete[] mesh.vertices;
    delete[] mesh.indices;
    delete[] mesh.wireframeIndices;

    mesh.vertices = nullptr;
    mesh.indices = nullptr;
    mesh.wireframeIndices = nullptr;

    mesh.vertexCount = mesh.indexCount = mesh.wireframeIndexCount = 0;
}