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

int main() {
    PointLight light = createDefaultLight(0, 5, 0); // light above origin

    Vector3 surface(0, 0, 0);
    Vector3 normal(0, 1, 0);       // flat surface facing up
    Vector3 material(1, 0.5, 0);   // orange material

    Vector3 result = calculatePointLIght(light, surface, normal, material);
    printf("Light result: %.3f, %.3f, %.3f\n", result.getX(), result.getY(), result.getZ());

    // Move light and test again
    translateLight(light, 10, 0, 0); // move far sideways
    result = calculatePointLIght(light, surface, normal, material);
    printf("After translate: %.3f, %.3f, %.3f\n", result.getX(), result.getY(), result.getZ());

    resetLight(light);
    printf("After reset, pos y: %.3f, %.3f, %.3f\n", light.position.getX(), light.position.getY(), light.position.getZ()); // should be 5
}
