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
#include "MathUtils.h"
#include "Shape.h"
#include "ShapeBuilder.h"
#include "Course.h"
#include "Windmill.h"

using namespace glm;
using namespace std;

const char *getError()
{
    const char *errorDescription = nullptr;
    glfwGetError(&errorDescription);
    return errorDescription ? errorDescription : "Unknown GLFW error (no description)";
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
    GLenum err = glewInit();
    // GLEW_ERROR_NO_GLX_DISPLAY is a known harmless error on Wayland/WSL2
    // (GLEW cannot enumerate extensions via GLX, but the EGL context is valid).
    if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY)
    {
        cout << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        throw "GLEW initialisation failed";
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
    GLFWwindow *window;
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

static void printControls()
{
    cout << "Controls:" << endl;
    cout << "  W/S  rotate X" << endl;
    cout << "  A/D  rotate Y" << endl;
    cout << "  Q/E  rotate Z" << endl;
    cout << "  I/K  move Y" << endl;
    cout << "  J/L  move X" << endl;
    cout << "  U/O  move Z" << endl;
    cout << "  + / -  change blade speed" << endl;
    cout << "  Enter  toggle wireframe" << endl;
}

int main()
{
    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << "Setup failed: " << e << endl;
        return 1;
    }

    printControls();

    // One-time setup
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

    GLuint shaderID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    GLint viewLoc  = glGetUniformLocation(shaderID, "view");
    GLint projLoc  = glGetUniformLocation(shaderID, "projection");

    Course course;
    course.build();
    Windmill windmill;
    windmill.build();

    Matrix4 proj = Matrix4::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    Matrix4 view = Matrix4::lookAt(Vector3(0, 4, 7), Vector3(0, 0, 0), Vector3(0, 1, 0));
    glUseProgram(shaderID);
    glUniformMatrix4fv(projLoc, 1, GL_TRUE, proj.getData());
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view.getData());

    Matrix4 sceneTransform = Matrix4::identity();
    bool wireframe = false;
    double lastTime = glfwGetTime();

    float step = 1.0f;       // degrees/frame for rotation
    float m = 0.05f;         // units/frame for translation
    float speedStep = 10.0f; // speed change per key press

    bool enterWasDown = false;
    bool plusWasDown = false;
    bool minusWasDown = false;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // 1. Delta time
        double currentTime = glfwGetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;

        // 2. Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderID);

        // 3. Rotation keys (LEFT-multiply = world space)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateX( step).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateX(-step).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateY(-step).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateY( step).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateZ( step).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            sceneTransform = Matrix4::rotateZ(-step).multiply(sceneTransform);

        // 4. Translation keys
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            sceneTransform = Matrix4::translate(0,  m, 0).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            sceneTransform = Matrix4::translate(0, -m, 0).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            sceneTransform = Matrix4::translate( m, 0, 0).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            sceneTransform = Matrix4::translate(-m, 0, 0).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            sceneTransform = Matrix4::translate(0, 0,  m).multiply(sceneTransform);
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            sceneTransform = Matrix4::translate(0, 0, -m).multiply(sceneTransform);

        // 5. Blade speed (+/-)
        bool plusDown = glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
                        glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS;
        if (plusDown && !plusWasDown)
            windmill.bladeSpeed += speedStep;
        plusWasDown = plusDown;

        bool minusDown = glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
                         glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS;
        if (minusDown && !minusWasDown) {
            windmill.bladeSpeed -= speedStep;
            if (windmill.bladeSpeed < 0.0f)
                windmill.bladeSpeed = 0.0f;
        }
        minusWasDown = minusDown;

        // 6. Wireframe toggle (edge-triggered)
        bool enterDown = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
        if (enterDown && !enterWasDown) {
            wireframe = !wireframe;
        }
        enterWasDown = enterDown;

        // 7. Update + draw
        windmill.update(dt);
        course.draw(shaderID, sceneTransform, wireframe);
        windmill.draw(shaderID, sceneTransform, wireframe);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
