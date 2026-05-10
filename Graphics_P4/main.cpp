#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "light.h"
#include "texture.h"
#include "MathUtils.h"
#include "sphere.h"
#include "plane.h"
#include "scene.h"

using namespace glm;
using namespace std;

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                             // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1000, 1000, "u23588579", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
}

//setting funcs for testing purposes
void testPointLight();
void testSphereMesh();
void testPlaneMesh(bool hasGL);

int main() {

    GLFWwindow *window = nullptr;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << e << endl;
    }

    // TODO: scene wiring goes here (shader, uniforms, draw loop)

    // testPointLight();
    // testSphereMesh();
    testPlaneMesh(window != nullptr);

    //now wire up the scene and draw the plane and sphere using the shader, then call cleanupSphere and cleanupPlane before exiting

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    SceneState state;

    state = createInitialState(0, 0, 0); // light at origin for testing 

    

    return 0;
}

void testPointLight() {
    PointLight light = createDefaultLight(0, 5, 0); // light above origin
    printf("Initial position: %.3f, %.3f, %.3f\n", light.position.getX(), light.position.getY(), light.position.getZ()); // should be (0, 5, 0)
    printf("Initial colour: %.3f, %.3f, %.3f\n", light.r, light.g, light.b); // should be (1, 1, 1)
    printf("Initial intensity: %.3f\n", light.intensity); // should be 2.0

    translateLight(light, 10, 0, 0); // move far sideways
    printf("After translate: %.3f, %.3f, %.3f\n", light.position.getX(), light.position.getY(), light.position.getZ()); // should be (10, 5, 0)

    setLightColour(light, 0.5f, 0.5f, 1.0f); // change to a blueish colour
    printf("After colour change: %.3f, %.3f, %.3f\n", light.r, light.g, light.b); // should be (0.5, 0.5, 1)

    resetLight(light);
    printf("After reset: position: %.3f, %.3f, %.3f\n", light.position.getX(), light.position.getY(), light.position.getZ()); // should be (0, 5, 0)
    printf("After reset: colour: %.3f, %.3f, %.3f\n", light.r, light.g, light.b); // should be (1, 1, 1)
    printf("After reset: intensity: %.3f\n", light.intensity); // should be 2.0
}

void testSphereMesh() {
    SphereMesh mesh = generateSphere(4, 4, 1.0f);
    std::cout << "Num of stacks: " << mesh.currentStacks << std::endl; // should be 4
    std::cout << "Num of sectors: " << mesh.currentSectors << std::endl; // should be 4
    std::cout << "Radius: " << mesh.radius << std::endl; // should be 1.0
    printf("Sphere vertex count: %d\n", mesh.vertexCount); // should be 25 (5x5)
    printf("Sphere triangle index count: %d\n", mesh.indexCount); // should be 96 (4x4x6)
    printf("Sphere wireframe index count: %d\n", mesh.wireframeIndexCount); // should be 64 (4x4x4)
    cleanupSphere(mesh);
}

void testPlaneMesh(bool hasGL) {
    printf("\n--- testPlaneMesh ---\n");
    int N = 4;
    float size = 2.0f;
    PlaneMesh mesh = generatePlane(N, size);

    // Count checks
    printf("Resolution: %d (expect %d)\n", mesh.currentResolution, N);
    printf("Size: %.1f (expect %.1f)\n", mesh.size, size);
    printf("Vertex count: %d (expect %d)\n", mesh.vertexCount, (N+1)*(N+1));        // 25
    printf("Triangle index count: %d (expect %d)\n", mesh.indexCount, N*N*6);        // 96
    printf("Wireframe index count: %d (expect %d)\n", mesh.wireframeIndexCount, N*N*8); // 128

    // Corner vertex positions  (negReach = -1, stepSize = 0.5 for size=2, N=4)
    PlaneVertex& v00 = mesh.vertices[0];               // i=0, j=0
    PlaneVertex& v0N = mesh.vertices[N];               // i=0, j=N
    PlaneVertex& vNN = mesh.vertices[N*(N+1) + N];     // i=N, j=N
    printf("v[0,0] pos:    (%.2f, %.2f, %.2f) expect (-1.00, 0.00, -1.00)\n", v00.x, v00.y, v00.z);
    printf("v[0,N] pos:    (%.2f, %.2f, %.2f) expect ( 1.00, 0.00, -1.00)\n", v0N.x, v0N.y, v0N.z);
    printf("v[N,N] pos:    (%.2f, %.2f, %.2f) expect ( 1.00, 0.00,  1.00)\n", vNN.x, vNN.y, vNN.z);
    printf("v[0,0] normal: (%.0f, %.0f, %.0f)       expect (0, 1, 0)\n", v00.nx, v00.ny, v00.nz);
    printf("v[0,0] uv:     (%.2f, %.2f)           expect (0.00, 0.00)\n", v00.u, v00.v);
    printf("v[N,N] uv:     (%.2f, %.2f)           expect (1.00, 1.00)\n", vNN.u, vNN.v);

    // Triangle indices for first quad (i=0, j=0): topLeft=0, bottomLeft=5, topRight=1, bottomRight=6
    printf("tri[0]: %u %u %u (expect 0 5 1)\n", mesh.indices[0], mesh.indices[1], mesh.indices[2]);
    printf("tri[1]: %u %u %u (expect 1 5 6)\n", mesh.indices[3], mesh.indices[4], mesh.indices[5]);

    // Wireframe indices for first quad: k1=0, k2=5, k3=1, k4=6  ->  0 5 5 6 6 1 1 0
    printf("wire[0]: %u %u %u %u %u %u %u %u (expect 0 5 5 6 6 1 1 0)\n",
        mesh.wireframeIndices[0], mesh.wireframeIndices[1],
        mesh.wireframeIndices[2], mesh.wireframeIndices[3],
        mesh.wireframeIndices[4], mesh.wireframeIndices[5],
        mesh.wireframeIndices[6], mesh.wireframeIndices[7]);

    // GPU handles should be 0 before upload
    printf("VAO before upload: %u (expect 0)\n", mesh.VAO);

    if (hasGL) {
        uploadPlaneToGPU(mesh);
        printf("VAO after upload: %u (expect non-zero)\n", mesh.VAO);
        printf("VBO after upload: %u (expect non-zero)\n", mesh.VBO);
        printf("EBO after upload: %u (expect non-zero)\n", mesh.EBO);
        printf("wireEBO after upload: %u (expect non-zero)\n", mesh.wireEBO);

        cleanupPlane(mesh);
        printf("VAO after cleanup: %u (expect 0)\n", mesh.VAO);
        printf("Vertex count after cleanup: %d (expect 0)\n", mesh.vertexCount);
        printf("vertices ptr after cleanup: %s (expect null)\n", mesh.vertices == nullptr ? "null" : "NOT null");
    } else {
        printf("(skipping GPU tests — no GL context)\n");
        cleanupPlane(mesh);
    }
}
