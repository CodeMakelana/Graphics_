#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "MathUtils.h"
#include "Course.h"
#include "Windmill.h"

using namespace std;

const char *getError() {
    const char *errorDescription = nullptr;
    glfwGetError(&errorDescription);
    return errorDescription ? errorDescription : "Unknown GLFW error";
}

inline GLFWwindow *setUp() {
    glewExperimental = true;
    if (!glfwInit()) throw getError();
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Window title must be your student number
    GLFWwindow *window = glfwCreateWindow(1000, 1000, "u23588579", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw "Failed to open GLFW window.";
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
        glfwTerminate();
        throw "GLEW initialisation failed";
    }
    return window;
}

int main() {
    GLFWwindow *window;
    try { window = setUp(); } catch (const char *e) { cout << e << endl; return 1; }

    glEnable(GL_DEPTH_TEST);
    // Distinct sky blue background color
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f); 

    GLuint shaderID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    GLint viewLoc  = glGetUniformLocation(shaderID, "view");
    GLint projLoc  = glGetUniformLocation(shaderID, "projection");

    Course course;
    course.build();
    Windmill windmill;
    windmill.build();

    // Setup camera matrices
    Matrix4 proj = Matrix4::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    Matrix4 view = Matrix4::lookAt(Vector3(0, 5, 8), Vector3(0, 0.8f, 0), Vector3(0, 1, 0));
    glUseProgram(shaderID);
    glUniformMatrix4fv(projLoc, 1, GL_TRUE, proj.getData());
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view.getData());

    // --- State Variables for Independent Transformations ---
    Matrix4 accumulatedRotation = Matrix4::rotateY(0.0f);

    float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    
    bool wireframe = false;
    double lastToggleTime = 0.0;
    double lastTime = glfwGetTime();

    float step = 1.5f;       // Rotation speed (degrees per frame)
    float m = 0.05f;         // Translation speed (units per frame)
    float speedStep = 45.0f; // Rotor acceleration

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderID);

        // --- Transformation Controls ---
        
        // Rotations: Left-multiply onto the accumulated matrix for Global axis rotation
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateX( step).multiply(accumulatedRotation);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateX(-step).multiply(accumulatedRotation);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateY( step).multiply(accumulatedRotation);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateY(-step).multiply(accumulatedRotation);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateZ(-step).multiply(accumulatedRotation);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) accumulatedRotation = Matrix4::rotateZ( step).multiply(accumulatedRotation);

        // Translations: Update coordinates
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) posY += m;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) posY -= m;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) posX += m;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) posX -= m;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) posZ += m;
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) posZ -= m;

        // Rebuild sceneTransform every frame: Translation * Accumulated Rotation
        Matrix4 transMat = Matrix4::translate(posX, posY, posZ);
        Matrix4 sceneTransform = transMat.multiply(accumulatedRotation);

        // --- Blade Speed Controls ---
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
            windmill.bladeSpeed += speedStep * dt;
        }
        
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
            windmill.bladeSpeed -= speedStep * dt;
            if (windmill.bladeSpeed < 0.0f) {
                windmill.bladeSpeed = 0.0f;
            }
        }

        // --- Wireframe Toggle ---
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (currentTime - lastToggleTime > 0.2) {
                wireframe = !wireframe;
                lastToggleTime = currentTime;
            }
        }

        // --- Update and Draw ---
        windmill.update(dt);
        course.draw(shaderID, sceneTransform, wireframe);
        windmill.draw(shaderID, sceneTransform, wireframe);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}