#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>

// ---------------------------------------------------------------------------
// SphereVertex
// Interleaved vertex format used by the golf ball.
//   position (x, y, z)   , 3 floats
//   normal   (nx, ny, nz), 3 floats
//   texcoord (u, v)      , 2 floats
// Total: 8 floats = 32 bytes per vertex.
// ---------------------------------------------------------------------------
struct SphereVertex {
    float x, y, z;    // world-space position (before transforms)
    float nx, ny, nz; // unit-length surface normal
    float u, v;       // texture coordinate in [0, 1]
};

// ---------------------------------------------------------------------------
// SphereMesh
// Holds both the CPU-side geometry and the GPU handles.
// Uses raw arrays (not std::vector) because <vector> is not in the allowed
// list of libraries for Practical 4.
// ---------------------------------------------------------------------------
struct SphereMesh {
    // CPU-side geometry (owned, must be freed by cleanupSphere)
    SphereVertex* vertices;
    unsigned int* indices;          // triangle indices, used with GL_TRIANGLES
    unsigned int* wireframeIndices; // line indices, used with GL_LINES

    int vertexCount;
    int indexCount;
    int wireframeIndexCount;

    // GPU handles
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;      // element buffer for triangle indices
    GLuint wireEBO;  // element buffer for wireframe line indices

    // Current parameters, used by updateSphereResolution to skip no-op rebuilds
    int currentStacks;
    int currentSectors;
    float radius;
};

// ---------------------------------------------------------------------------
// generateSphere
// Builds the CPU-side vertex and index arrays for a UV sphere using the
// latitude/longitude method.
//   stacks  , number of horizontal subdivisions (pole to pole)
//   sectors , number of vertical subdivisions (around the equator)
//   radius  , sphere radius in world units
// Does NOT upload to the GPU, call uploadSphereToGPU afterwards.
// ---------------------------------------------------------------------------
SphereMesh generateSphere(int stacks, int sectors, float radius);

// ---------------------------------------------------------------------------
// uploadSphereToGPU
// Creates the VAO, VBO, EBO, and wireEBO on the GPU and uploads the mesh data.
// Sets up three vertex attribute pointers:
//   location 0 , position (3 floats)
//   location 1 , normal   (3 floats)
//   location 2 , texcoord (2 floats)
// ---------------------------------------------------------------------------
void uploadSphereToGPU(SphereMesh& mesh);

// ---------------------------------------------------------------------------
// updateSphereResolution
// Regenerates the sphere with a new vertex count and re-uploads to the GPU.
// Called when the user presses the vertex-count cycling keys in main.cpp.
// If the new values match the current ones, this is a no-op.
// Frees old CPU arrays and GPU buffers before rebuilding.
// ---------------------------------------------------------------------------
void updateSphereResolution(SphereMesh& mesh, int newStacks, int newSectors, float radius);

// ---------------------------------------------------------------------------
// drawSphere
// Issues a draw call. Binds the correct element buffer based on the wireframe
// flag (EBO for triangles, wireEBO for lines) and calls glDrawElements.
// Uses GL_LINES for wireframe, NOT glPolygonMode, as required by the spec.
// ---------------------------------------------------------------------------
void drawSphere(const SphereMesh& mesh, bool wireframe);

// ---------------------------------------------------------------------------
// cleanupSphere
// Deletes the GPU buffers and frees the CPU arrays.
// Called by updateSphereResolution before rebuilding, and by main.cpp at
// program shutdown.
// ---------------------------------------------------------------------------
void cleanupSphere(SphereMesh& mesh);

#endif // SPHERE_H