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
#include "MathUtils.h"
#include "sphere.h"

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
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
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

int main() {
   
    testPointLight();
    testSphereMesh();

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
    printf("Sphere vertex count: %d\n", mesh.vertexCount); // should be 121 (11x11)
    printf("Sphere triangle index count: %d\n", mesh.indexCount); // should be 600 (10x10x6)
    printf("Sphere wireframe index count: %d\n", mesh.wireframeIndexCount); // should be 400
    cleanupSphere(mesh);
}
