#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "Mat4.h"
#include "Vec3.h"
#include "Shape.h"
#include "Scene.h"

using namespace std;

// ─── Window ───────────────────────────────────────────────────────────────────
const int   WINDOW_WIDTH  = 1000;
const int   WINDOW_HEIGHT = 800;
const char* WINDOW_TITLE  = "U23588579";

// ─── Background colour ────────────────────────────────────────────────────────
const float BG_R = 0.10f, BG_G = 0.12f, BG_B = 0.10f;

// ─── Globals shared between callbacks and main ────────────────────────────────
GLint  g_transformLoc  = -1;
GLint  g_colourLoc     = -1;
bool   g_wireframeMode = false;
Scene* g_scene         = nullptr;

// How much to move/rotate/scale per key press
const float MOVE_STEP   = 0.05f;
const float ROTATE_STEP = 0.05f; // radians (~3 degrees)
const float SCALE_STEP  = 1.10f; // 10% larger per press
const float SCALE_DOWN  = 1.0f / 1.10f;

// Debounce timer for Enter key (prevents multiple toggles per press)
double g_lastEnterTime = 0.0;
const double ENTER_DEBOUNCE = 0.25; // seconds

// ─────────────────────────────────────────────────────────────────────────────
//  Shader helpers
// ─────────────────────────────────────────────────────────────────────────────

void setTransform(const Mat4& mat)
{
    float arr[16];
    mat.toArray(arr);
    glUniformMatrix4fv(g_transformLoc, 1, GL_FALSE, arr);
}

void setColour(const Vec3& colour)
{
    glUniform3f(g_colourLoc, colour.x, colour.y, colour.z);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Draw helpers
// ─────────────────────────────────────────────────────────────────────────────

void drawShape(const Shape& shape, bool wireframe)
{
    setColour(shape.getDrawColour());
    setTransform(shape.getTransform());

    if (wireframe)
        shape.drawWireframe();
    else
        shape.draw();
}

// Draws every shape in the scene
void drawScene(const Scene& scene, bool wireframe)
{
    // We iterate manually so each shape gets its own uniforms set
    // Scene::drawAll() is used here via the shapes indirectly through drawShape
    scene.drawAll(wireframe);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Callbacks
// ─────────────────────────────────────────────────────────────────────────────

void framebufferSizeCallback(GLFWwindow* /*w*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int /*scan*/, int action, int /*mods*/)
{
    // ── Close ─────────────────────────────────────────────────────────────────
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (!g_scene) return;

    // ── Selection keys (on press only) ────────────────────────────────────────
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_1: g_scene->selectBall();       return;
            case GLFW_KEY_2: g_scene->selectObstacle1();  return;
            case GLFW_KEY_3: g_scene->selectObstacle2();  return;
            case GLFW_KEY_4: g_scene->selectHole();       return;
            case GLFW_KEY_0: g_scene->deselectAll();      return;

            // Wireframe toggle — debounced so one press = one toggle
            case GLFW_KEY_ENTER:
            {
                double now = glfwGetTime();
                if (now - g_lastEnterTime > ENTER_DEBOUNCE)
                {
                    g_wireframeMode  = !g_wireframeMode;
                    g_lastEnterTime  = now;
                }
                return;
            }
            default: break;
        }
    }

    // ── Transform keys (on press AND held) ───────────────────────────────────
    // GLFW_PRESS fires once; GLFW_REPEAT fires while held.
    // Using both lets you tap for small moves or hold for continuous movement.
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    // Find the currently selected shape across all selectables
    Shape* sel = nullptr;
    // Check which shape is selected by checking the selected flag
    auto trySelect = [&](Shape* s) { if (s && s->selected) sel = s; };
    trySelect(g_scene->golfBall);
    trySelect(g_scene->obstacle1);
    trySelect(g_scene->obstacle2);
    trySelect(g_scene->golfHole);

    if (!sel) return; // nothing selected — ignore transform keys

    switch (key)
    {
        // Translation
        case GLFW_KEY_W: sel->translateY += MOVE_STEP;  break; // up
        case GLFW_KEY_S: sel->translateY -= MOVE_STEP;  break; // down
        case GLFW_KEY_A: sel->translateX -= MOVE_STEP;  break; // left
        case GLFW_KEY_D: sel->translateX += MOVE_STEP;  break; // right

        // Rotation (E = clockwise → negative angle in OpenGL's Y-up system)
        case GLFW_KEY_E: sel->rotation -= ROTATE_STEP;  break; // clockwise
        case GLFW_KEY_Q: sel->rotation += ROTATE_STEP;  break; // counter-clockwise

        // Scale (from centre of shape)
        case GLFW_KEY_EQUAL:     // + key (no shift needed)
        case GLFW_KEY_KP_ADD:    // numpad +
            sel->scaleX *= SCALE_STEP;
            sel->scaleY *= SCALE_STEP;
            break;

        case GLFW_KEY_MINUS:     // - key
        case GLFW_KEY_KP_SUBTRACT:
            sel->scaleX *= SCALE_DOWN;
            sel->scaleY *= SCALE_DOWN;
            break;

        default: break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Initialisation
// ─────────────────────────────────────────────────────────────────────────────

const char* getGLFWError()
{
    const char* d; glfwGetError(&d);
    return d ? d : "Unknown GLFW error";
}

void startUpGLFW() { if (!glfwInit()) throw getGLFWError(); }

void startUpGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        cerr << "GLEW warning: " << glewGetErrorString(err) << endl;
    glGetError();
}

GLFWwindow* setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES,               4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!w) { cerr << getGLFWError() << endl; glfwTerminate(); throw "Window creation failed."; }

    glfwMakeContextCurrent(w);
    startUpGLEW();
    glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
    glfwSetKeyCallback(w, keyCallback);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    GLFWwindow* window = nullptr;
    try { window = setUp(); }
    catch (const char* e) { cerr << e << endl; return EXIT_FAILURE; }

    // ── Shaders ──────────────────────────────────────────────────────────────
    GLuint prog = LoadShaders("vertex.glsl", "fragment.glsl");
    if (prog == 0) { cerr << "Shader load failed." << endl; glfwTerminate(); return EXIT_FAILURE; }
    glUseProgram(prog);

    g_transformLoc = glGetUniformLocation(prog, "transform");
    g_colourLoc    = glGetUniformLocation(prog, "colour");
    if (g_transformLoc == -1) cerr << "Warning: transform uniform not found.\n";
    if (g_colourLoc    == -1) cerr << "Warning: colour uniform not found.\n";

    // ── Build scene ───────────────────────────────────────────────────────────
    Scene scene;
    scene.init();       // upload all shapes to GPU
    g_scene = &scene;   // expose to keyboard callback

    glClearColor(BG_R, BG_G, BG_B, 1.0f);
    glfwSwapInterval(1);

    // ── Render loop ───────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw every shape — each shape sets its own colour and transform uniforms
        for (Shape* s : scene.getShapes())
            drawShape(*s, g_wireframeMode);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    glDeleteProgram(prog);
    glfwTerminate();
    return EXIT_SUCCESS;
}