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

// Global — GLFW key callbacks are static and cannot capture locals
SceneState state;

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
    GLenum glewErr = glewInit();
    // GLEW_ERROR_NO_GLX_DISPLAY (4) is a false positive when GLFW owns the context;
    // GL functions are still loaded. Only abort on errors that actually break loading.
    if (glewErr != GLEW_OK && glewErr != 4)
    {
        glfwTerminate();
        throw "GLEW failed to initialise.\n";
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
        const char *err = getError();
        fprintf(stderr, "glfwCreateWindow failed: %s\n", err ? err : "(no description)");
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        resetScene(state);

    // Sphere rotation
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotX += 2.0f;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotX -= 2.0f;
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotY -= 2.0f;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotY += 2.0f;
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotZ -= 2.0f;
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotZ += 2.0f;

    // Wireframe toggle — 300 ms debounce prevents flicker from key repeat
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        double currentTime = glfwGetTime();
        if (currentTime - state.lastWireframeToggle > 0.3)
        {
            state.wireframeEnabled = !state.wireframeEnabled;
            state.lastWireframeToggle = currentTime;
        }
    }

    // Light translation — arrow keys move x/y, comma/period move z
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, 0.0f, 0.1f, 0.0f);
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, 0.0f, -0.1f, 0.0f);
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, 0.1f, 0.0f, 0.0f);
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, -0.1f, 0.0f, 0.0f);
    if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, 0.0f, 0.0f, 0.1f);   // > key (unshifted .)
    if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, 0.0f, 0.0f, -0.1f);  // < key (unshifted ,)

    // Alpha ([ / ])
    if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_PRESS || action == GLFW_REPEAT))
        adjustAlpha(state, 0.05f);
    if (key == GLFW_KEY_LEFT_BRACKET && (action == GLFW_PRESS || action == GLFW_REPEAT))
        adjustAlpha(state, -0.05f);

    // Floor colour (N / M)
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        cycleFloorColor(state, -1);
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        cycleFloorColor(state, 1);

    // Ball colour (Z / X)
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        cycleBallColor(state, -1);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        cycleBallColor(state, 1);

    // Light colour (K / L)
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
        cycleLightColor(state, -1);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        cycleLightColor(state, 1);

    // Texture toggles
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        state.colourTexEnabled = !state.colourTexEnabled;
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        state.displacementTexEnabled = !state.displacementTexEnabled;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        state.alphaTexEnabled = !state.alphaTexEnabled;

}

void testPointLight();
void testSphereMesh();
void testPlaneMesh(bool hasGL);

int main()
{
    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cerr << (e ? e : "Unknown setup error") << endl;
        return -1;
    }

    // ---- Add code here ----

    state = createInitialState(0.0f, 1.0f, 0.0f);

    GLuint sphereShader = LoadShaders("sphere.vert", "sphere.frag");
    GLuint floorShader  = LoadShaders("floor.vert",  "floor.frag");

    SphereMesh ball  = generateSphere(state.sphereStacks, state.sphereSectors, 1.0f);
    uploadSphereToGPU(ball);

    PlaneMesh ground = generatePlane(state.floorResolution, 1000.0f);
    uploadPlaneToGPU(ground);

    GLuint colourTex       = createColourTexture(512, 512);
    GLuint displacementTex = createDisplacementTexture(512, 512);
    GLuint alphaTex        = createAlphaTexture(512, 512);

    glfwSetKeyCallback(window, keyCallback);

    // Fixed camera
    Matrix4 view = Matrix4::lookAt(
        Vector3(0.0f, 3.0f, 8.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f)
    );
    Matrix4 proj = Matrix4::perspective(45.0f, 1.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateSphereResolution(ball, state.sphereStacks, state.sphereSectors, 1.0f);

        // Build model matrix from current rotations
        Matrix4 model = Matrix4::translate(0.0f, 1.0f, 0.0f)
            .multiply(Matrix4::rotateX(state.rotX)
            .multiply(Matrix4::rotateY(state.rotY)
            .multiply(Matrix4::rotateZ(state.rotZ))));

        // --- Draw floor ---
        glUseProgram(floorShader);
        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uModel"), 1, GL_TRUE,
            Matrix4::translate(0.0f, -0.001f, 0.0f).getData());
        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uView"),       1, GL_TRUE, view.getData());
        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uProjection"), 1, GL_TRUE, proj.getData());
        glUniform3f(glGetUniformLocation(floorShader, "uLightPos"),
            state.light.position.getX(),
            state.light.position.getY(),
            state.light.position.getZ());
        glUniform3f(glGetUniformLocation(floorShader, "uLightColour"),
            state.light.r, state.light.g, state.light.b);
        glUniform1f(glGetUniformLocation(floorShader, "uLightIntensity"), state.light.intensity);
        glUniform3f(glGetUniformLocation(floorShader, "uFloorColour"),
            state.floorColors[state.floorColorIndex][0],
            state.floorColors[state.floorColorIndex][1],
            state.floorColors[state.floorColorIndex][2]);
        glUniform4f(glGetUniformLocation(floorShader, "uBallColour"),
            state.ballColors[state.ballColorIndex][0],
            state.ballColors[state.ballColorIndex][1],
            state.ballColors[state.ballColorIndex][2],
            state.alpha);
        drawPlane(ground, false);

        // --- Draw sphere ---
        glUseProgram(sphereShader);
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uModel"),      1, GL_TRUE, model.getData());
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uView"),       1, GL_TRUE, view.getData());
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uProjection"), 1, GL_TRUE, proj.getData());
        bindTexture(colourTex,       0, sphereShader, "uColourTex");
        bindTexture(displacementTex, 1, sphereShader, "uDisplacementTex");
        bindTexture(alphaTex,        2, sphereShader, "uAlphaTex");
        glUniform1i(glGetUniformLocation(sphereShader, "uColourTexEnabled"),     state.colourTexEnabled);
        glUniform1i(glGetUniformLocation(sphereShader, "uDisplacementEnabled"),  state.displacementTexEnabled);
        glUniform1i(glGetUniformLocation(sphereShader, "uAlphaTexEnabled"),      state.alphaTexEnabled);
        glUniform1f(glGetUniformLocation(sphereShader, "uDisplacementStrength"), 0.05f);
        glUniform4f(glGetUniformLocation(sphereShader, "uBallColour"),
            state.ballColors[state.ballColorIndex][0],
            state.ballColors[state.ballColorIndex][1],
            state.ballColors[state.ballColorIndex][2],
            state.alpha);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSphere(ball, state.wireframeEnabled);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanupSphere(ball);
    cleanupPlane(ground);
    glDeleteTextures(1, &colourTex);
    glDeleteTextures(1, &displacementTex);
    glDeleteTextures(1, &alphaTex);
    glDeleteProgram(sphereShader);
    glDeleteProgram(floorShader);
    glfwDestroyWindow(window);
    glfwTerminate();
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

    printf("Resolution: %d (expect %d)\n", mesh.currentResolution, N);
    printf("Size: %.1f (expect %.1f)\n", mesh.size, size);
    printf("Vertex count: %d (expect %d)\n", mesh.vertexCount, (N+1)*(N+1));
    printf("Triangle index count: %d (expect %d)\n", mesh.indexCount, N*N*6);
    printf("Wireframe index count: %d (expect %d)\n", mesh.wireframeIndexCount, N*N*8);

    PlaneVertex& v00 = mesh.vertices[0];
    PlaneVertex& v0N = mesh.vertices[N];
    PlaneVertex& vNN = mesh.vertices[N*(N+1) + N];
    printf("v[0,0] pos:    (%.2f, %.2f, %.2f) expect (-1.00, 0.00, -1.00)\n", v00.x, v00.y, v00.z);
    printf("v[0,N] pos:    (%.2f, %.2f, %.2f) expect ( 1.00, 0.00, -1.00)\n", v0N.x, v0N.y, v0N.z);
    printf("v[N,N] pos:    (%.2f, %.2f, %.2f) expect ( 1.00, 0.00,  1.00)\n", vNN.x, vNN.y, vNN.z);
    printf("v[0,0] normal: (%.0f, %.0f, %.0f)       expect (0, 1, 0)\n", v00.nx, v00.ny, v00.nz);
    printf("v[0,0] uv:     (%.2f, %.2f)           expect (0.00, 0.00)\n", v00.u, v00.v);
    printf("v[N,N] uv:     (%.2f, %.2f)           expect (1.00, 1.00)\n", vNN.u, vNN.v);

    printf("tri[0]: %u %u %u (expect 0 5 1)\n", mesh.indices[0], mesh.indices[1], mesh.indices[2]);
    printf("tri[1]: %u %u %u (expect 1 5 6)\n", mesh.indices[3], mesh.indices[4], mesh.indices[5]);
    printf("wire[0]: %u %u %u %u %u %u %u %u (expect 0 5 5 6 6 1 1 0)\n",
        mesh.wireframeIndices[0], mesh.wireframeIndices[1],
        mesh.wireframeIndices[2], mesh.wireframeIndices[3],
        mesh.wireframeIndices[4], mesh.wireframeIndices[5],
        mesh.wireframeIndices[6], mesh.wireframeIndices[7]);

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
