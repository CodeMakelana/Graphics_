#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

using namespace std;

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
const float FLOOR_SIZE    = 20.0f;
const float SPHERE_RADIUS = 1.0f;
const float SPHERE_Y      = 1.0f; // sphere centre height above floor

// ---------------------------------------------------------------------------
// Globals — GLFW key callbacks are static and cannot capture locals,
// so the scene state and meshes must be accessible at file scope.
// ---------------------------------------------------------------------------
SceneState state;
SphereMesh ball;
PlaneMesh  ground;

// ---------------------------------------------------------------------------
// GLFW error helpers
// ---------------------------------------------------------------------------
const char* getError()
{
    const char* errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true;
    if (!glfwInit())
        throw getError();
}

inline void startUpGLEW()
{
    glewExperimental = true;
    GLenum glewErr = glewInit();
    // GLEW_ERROR_NO_GLX_DISPLAY (4) is a false positive on WSL2 when GLFW owns
    // the context; GL functions are still loaded. Only abort on real failures.
    if (glewErr != GLEW_OK && glewErr != 4)
    {
        glfwTerminate();
        throw "GLEW failed to initialise.\n";
    }
}

inline GLFWwindow* setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "u23588579", NULL, NULL);
    if (window == NULL)
    {
        const char* err = getError();
        fprintf(stderr, "glfwCreateWindow failed: %s\n", err ? err : "(no description)");
        glfwTerminate();
        throw "Failed to open GLFW window.\n";
    }

    glfwMakeContextCurrent(window);
    startUpGLEW();
    return window;
}

// ---------------------------------------------------------------------------
// Key callback
// All spec-required key bindings live here.
//
// Rotation (spec section 3.5.1):
//   W  → rotate scene about Z-axis
//   S  → rotate scene about X-axis
//   A  → rotate scene about Y-axis
//   D  → rotate scene about Y-axis (opposite to A)
//   E  → rotate scene about Z-axis
//   Q  → rotate scene about Z-axis (opposite to E)
//
// Light translation (spec section 3.5.2):
//   UP    → light +Y
//   DOWN  → light -Y
//   RIGHT → light +X
//   LEFT  → light -X
//   .     → light +Z  (the > key, unshifted)
//   ,     → light -Z  (the < key, unshifted)
//
// Alpha (spec section 3.3):
//   +  → increase alpha
//   -  → decrease alpha
//
// Texture toggles (spec section 3.2):
//   B  → colour texture on/off
//   N  → displacement texture on/off
//   M  → alpha texture on/off
//
// Colour cycling (spec section 3.3):
//   T / Y  → floor colour prev / next
//   Z / X  → ball colour  prev / next
//   K / L  → light colour prev / next
//
// Vertex count (spec section 3.1):
//   F / G  → sphere vertex count down / up
//   H / J  → floor vertex count  down / up
//
// Wireframe (spec section 3.6):
//   Enter  → toggle wireframe (300 ms debounce)
//
// Reset (spec section 3.5.3):
//   Space  → reset entire scene to initial state
// ---------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // ---- Reset ----
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        resetScene(state);
        // Rebuild meshes to match the reset vertex counts
        updateSphereResolution(ball,   state.sphereStacks,   state.sphereSectors, SPHERE_RADIUS);
        updatePlaneResolution (ground, state.floorResolution, FLOOR_SIZE);
    }

    // ---- Rotations (spec 3.5.1) ----
    // W: X-axis
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotX += 2.0f;
    // S: X-axis opposite to W
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotX -= 2.0f;
    // A: Y-axis
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotY -= 2.0f;
    // D: Y-axis opposite to A
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotY += 2.0f;
    // E: Z-axis
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotZ += 2.0f;
    // Q: Z-axis opposite to E
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        state.rotZ -= 2.0f;

    // ---- Light translation (spec 3.5.2) ----
    if (key == GLFW_KEY_UP    && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light,  0.0f,  0.1f,  0.0f);
    if (key == GLFW_KEY_DOWN  && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light,  0.0f, -0.1f,  0.0f);
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light,  0.1f,  0.0f,  0.0f);
    if (key == GLFW_KEY_LEFT  && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light, -0.1f,  0.0f,  0.0f);
    // > key (unshifted period) → +Z
    if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light,  0.0f,  0.0f,  0.1f);
    // < key (unshifted comma) → -Z
    if (key == GLFW_KEY_COMMA  && (action == GLFW_PRESS || action == GLFW_REPEAT))
        translateLight(state.light,  0.0f,  0.0f, -0.1f);

    // ---- Alpha — + and - keys (spec 3.3) ----
    // On most keyboards + is GLFW_KEY_EQUAL (without shift), numpad + is KP_ADD
    if ((key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD)
        && (action == GLFW_PRESS || action == GLFW_REPEAT))
        adjustAlpha(state,  0.05f);
    if ((key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT)
        && (action == GLFW_PRESS || action == GLFW_REPEAT))
        adjustAlpha(state, -0.05f);

    // ---- Texture toggles — spec-mandated keys B / N / M (spec 3.2) ----
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        state.colourTexEnabled = !state.colourTexEnabled;
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        state.displacementTexEnabled = !state.displacementTexEnabled;
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        state.alphaTexEnabled = !state.alphaTexEnabled;

    // ---- Floor colour — T (prev) / Y (next) ----
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        cycleFloorColor(state, -1);
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
        cycleFloorColor(state,  1);

    // ---- Ball colour — Z (prev) / X (next) ----
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        cycleBallColor(state, -1);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        cycleBallColor(state,  1);

    // ---- Light colour — K (prev) / L (next) ----
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
        cycleLightColor(state, -1);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        cycleLightColor(state,  1);

    // ---- Sphere vertex count — F (fewer) / G (more) ----
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        // Minimum 6 stacks and sectors so the sphere stays recognisable
        if (state.sphereStacks   > 6) state.sphereStacks   -= 2;
        if (state.sphereSectors  > 6) state.sphereSectors  -= 2;
        updateSphereResolution(ball, state.sphereStacks, state.sphereSectors, SPHERE_RADIUS);
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        // Maximum 60 to keep rebuild fast
        if (state.sphereStacks  < 60) state.sphereStacks  += 2;
        if (state.sphereSectors < 60) state.sphereSectors += 2;
        updateSphereResolution(ball, state.sphereStacks, state.sphereSectors, SPHERE_RADIUS);
    }

    // ---- Floor vertex count — H (fewer) / J (more) ----
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if (state.floorResolution > 2) state.floorResolution -= 2;
        updatePlaneResolution(ground, state.floorResolution, FLOOR_SIZE);
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (state.floorResolution < 50) state.floorResolution += 2;
        updatePlaneResolution(ground, state.floorResolution, FLOOR_SIZE);
    }

    // ---- Wireframe toggle — Enter key, 300 ms debounce (spec 3.6) ----
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        double now = glfwGetTime();
        if (now - state.lastWireframeToggle > 0.3)
        {
            state.wireframeEnabled    = !state.wireframeEnabled;
            state.lastWireframeToggle = now;
        }
    }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
    GLFWwindow* window;
    try
    {
        window = setUp();
    }
    catch (const char* e)
    {
        fprintf(stderr, "Fatal: %s\n", e ? e : "unknown error");
        return -1;
    }

    // ---- Initialise scene state ----
    // Light starts at the sphere's centre (0, SPHERE_Y, 0)
    state = createInitialState(0.0f, SPHERE_Y, 0.0f);

    // ---- Load shaders ----
    GLuint sphereShader = LoadShaders("sphere.vert", "sphere.frag");
    GLuint floorShader  = LoadShaders("floor.vert",  "floor.frag");

    // ---- Build geometry ----
    ball   = generateSphere(state.sphereStacks,   state.sphereSectors, SPHERE_RADIUS);
    uploadSphereToGPU(ball);

    ground = generatePlane(state.floorResolution, FLOOR_SIZE);
    uploadPlaneToGPU(ground);

    // ---- Create procedural textures ----
    GLuint colourTex       = createColourTexture(512, 512);
    GLuint displacementTex = createDisplacementTexture(512, 512);
    GLuint alphaTex        = createAlphaTexture(512, 512);

    // ---- Register key callback ----
    glfwSetKeyCallback(window, keyCallback);

    // ---- Fixed camera ----
    // Eye at (0, 3, 8), looking at the sphere centre (0, SPHERE_Y, 0)
    Matrix4 view = Matrix4::lookAt(
        Vector3(0.0f, 3.0f, 8.0f),
        Vector3(0.0f, SPHERE_Y, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f)
    );
    // Square window → aspect ratio 1.0
    Matrix4 proj = Matrix4::perspective(45.0f, 1.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    // ====================================================================
    // Render loop
    // ====================================================================
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ----------------------------------------------------------------
        // Build the scene rotation matrix from current state angles.
        // Both the sphere and the floor share this rotation so the whole
        // scene appears to spin together (spec 3.5.1).
        // The light is NOT included here — it moves in world space only.
        // ----------------------------------------------------------------
        Matrix4 sceneRotation =
            Matrix4::rotateX(state.rotX)
            .multiply(Matrix4::rotateY(state.rotY)
            .multiply(Matrix4::rotateZ(state.rotZ)));

        // Sphere: rotate around its own centre first, then lift to y=SPHERE_Y
        Matrix4 sphereModel =
            Matrix4::translate(0.0f, SPHERE_Y, 0.0f)
            .multiply(sceneRotation);

        // Floor: same rotation, tiny downward offset to prevent z-fighting
        // at the sphere's contact circle
        Matrix4 floorModel =
            Matrix4::translate(0.0f, -0.001f, 0.0f)
            .multiply(sceneRotation);

        // ----------------------------------------------------------------
        // Draw floor
        // The floor shader computes per-fragment point-light illumination
        // using the Chapter-4 pseudocode.
        // ----------------------------------------------------------------
        glUseProgram(floorShader);

        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uModel"),
            1, GL_TRUE, floorModel.getData());
        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uView"),
            1, GL_TRUE, view.getData());
        glUniformMatrix4fv(glGetUniformLocation(floorShader, "uProjection"),
            1, GL_TRUE, proj.getData());

        // Light uniforms — position is in world space (not rotated with scene)
        glUniform3f(glGetUniformLocation(floorShader, "uLightPos"),
            state.light.position.getX(),
            state.light.position.getY(),
            state.light.position.getZ());
        glUniform3f(glGetUniformLocation(floorShader, "uLightColour"),
            state.light.r, state.light.g, state.light.b);
        glUniform1f(glGetUniformLocation(floorShader, "uLightIntensity"),
            state.light.intensity);

        // Floor and ball colour uniforms
        glUniform3f(glGetUniformLocation(floorShader, "uFloorColour"),
            state.floorColors[state.floorColorIndex][0],
            state.floorColors[state.floorColorIndex][1],
            state.floorColors[state.floorColorIndex][2]);
        glUniform4f(glGetUniformLocation(floorShader, "uBallColour"),
            state.ballColors[state.ballColorIndex][0],
            state.ballColors[state.ballColorIndex][1],
            state.ballColors[state.ballColorIndex][2],
            state.alpha);

        // Wireframe flag applies to the whole scene (spec 3.6)
        drawPlane(ground, state.wireframeEnabled);

        // ----------------------------------------------------------------
        // Draw sphere
        // ----------------------------------------------------------------
        glUseProgram(sphereShader);

        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uModel"),
            1, GL_TRUE, sphereModel.getData());
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uView"),
            1, GL_TRUE, view.getData());
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "uProjection"),
            1, GL_TRUE, proj.getData());

        // Bind the three textures to units 0, 1, 2
        bindTexture(colourTex,       0, sphereShader, "uColourTex");
        bindTexture(displacementTex, 1, sphereShader, "uDisplacementTex");
        bindTexture(alphaTex,        2, sphereShader, "uAlphaTex");

        // Texture enable flags (B / N / M keys)
        glUniform1i(glGetUniformLocation(sphereShader, "uColourTexEnabled"),
            state.colourTexEnabled);
        glUniform1i(glGetUniformLocation(sphereShader, "uDisplacementEnabled"),
            state.displacementTexEnabled);
        glUniform1i(glGetUniformLocation(sphereShader, "uAlphaTexEnabled"),
            state.alphaTexEnabled);

        // Displacement strength — tunable constant
        glUniform1f(glGetUniformLocation(sphereShader, "uDisplacementStrength"), 0.05f);

        // Ball colour + alpha (RGBA)
        glUniform4f(glGetUniformLocation(sphereShader, "uBallColour"),
            state.ballColors[state.ballColorIndex][0],
            state.ballColors[state.ballColorIndex][1],
            state.ballColors[state.ballColorIndex][2],
            state.alpha);

        // Enable alpha blending so the semi-transparent glass ball shows
        // the floor and light through it
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSphere(ball, state.wireframeEnabled);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---- Cleanup ----
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

// ---------------------------------------------------------------------------
// Unit tests — called manually during development, not in the render loop
// ---------------------------------------------------------------------------
void testPointLight()
{
    PointLight light = createDefaultLight(0, 5, 0);
    printf("Initial position: %.3f %.3f %.3f (expect 0 5 0)\n",
        light.position.getX(), light.position.getY(), light.position.getZ());
    printf("Initial colour:   %.3f %.3f %.3f (expect 1 1 1)\n",
        light.r, light.g, light.b);
    printf("Initial intensity: %.3f (expect 2.0)\n", light.intensity);

    translateLight(light, 10, 0, 0);
    printf("After translate: %.3f %.3f %.3f (expect 10 5 0)\n",
        light.position.getX(), light.position.getY(), light.position.getZ());

    setLightColour(light, 0.5f, 0.5f, 1.0f);
    printf("After colour: %.3f %.3f %.3f (expect 0.5 0.5 1)\n",
        light.r, light.g, light.b);

    resetLight(light);
    printf("After reset pos: %.3f %.3f %.3f (expect 0 5 0)\n",
        light.position.getX(), light.position.getY(), light.position.getZ());
    printf("After reset col: %.3f %.3f %.3f (expect 1 1 1)\n",
        light.r, light.g, light.b);
    printf("After reset int: %.3f (expect 2.0)\n", light.intensity);
}

void testSphereMesh()
{
    SphereMesh mesh = generateSphere(4, 4, 1.0f);
    printf("Stacks:   %d (expect 4)\n",  mesh.currentStacks);
    printf("Sectors:  %d (expect 4)\n",  mesh.currentSectors);
    printf("Radius:   %.1f (expect 1.0)\n", mesh.radius);
    printf("Vertices: %d (expect 25)\n",  mesh.vertexCount);
    printf("Indices:  %d (expect 96)\n",  mesh.indexCount);
    printf("WireIdx:  %d (expect 64)\n",  mesh.wireframeIndexCount);
    cleanupSphere(mesh);
}

void testPlaneMesh(bool hasGL)
{
    printf("\n--- testPlaneMesh ---\n");
    int   N    = 4;
    float size = 2.0f;
    PlaneMesh mesh = generatePlane(N, size);

    printf("Resolution: %d (expect %d)\n",   mesh.currentResolution, N);
    printf("Size:       %.1f (expect %.1f)\n", mesh.size, size);
    printf("Vertices:   %d (expect %d)\n",   mesh.vertexCount,         (N+1)*(N+1));
    printf("Tri idx:    %d (expect %d)\n",   mesh.indexCount,          N*N*6);
    printf("Wire idx:   %d (expect %d)\n",   mesh.wireframeIndexCount, N*N*8);

    PlaneVertex& v00 = mesh.vertices[0];
    PlaneVertex& v0N = mesh.vertices[N];
    PlaneVertex& vNN = mesh.vertices[N*(N+1)+N];
    printf("v[0,0] (%.2f %.2f %.2f) expect (-1 0 -1)\n", v00.x, v00.y, v00.z);
    printf("v[0,N] (%.2f %.2f %.2f) expect ( 1 0 -1)\n", v0N.x, v0N.y, v0N.z);
    printf("v[N,N] (%.2f %.2f %.2f) expect ( 1 0  1)\n", vNN.x, vNN.y, vNN.z);
    printf("normal (%.0f %.0f %.0f) expect (0 1 0)\n",   v00.nx, v00.ny, v00.nz);
    printf("uv[0,0] (%.2f %.2f) expect (0 0)\n", v00.u, v00.v);
    printf("uv[N,N] (%.2f %.2f) expect (1 1)\n", vNN.u, vNN.v);

    if (hasGL)
    {
        uploadPlaneToGPU(mesh);
        printf("VAO after upload: %u (expect non-zero)\n", mesh.VAO);
        cleanupPlane(mesh);
        printf("VAO after cleanup: %u (expect 0)\n", mesh.VAO);
        printf("vertices ptr: %s (expect null)\n",
            mesh.vertices == nullptr ? "null" : "NOT null");
    }
    else
    {
        printf("(skipping GPU tests — no GL context)\n");
        cleanupPlane(mesh);
    }
}
