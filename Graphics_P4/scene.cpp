#include "scene.h"

SceneState createInitialState(float lightCX, float lightCY, float lightCZ) {
    SceneState state;

    state.rotX = 0.0f;
    state.rotY = 0.0f;
    state.rotZ = 0.0f;

    state.sphereStacks   = 20;
    state.sphereSectors  = 20;
    state.floorResolution = 10;

    state.floorColorIndex = 0;
    state.ballColorIndex  = 0;
    state.lightColorIndex = 0;

    // Floor colours: Red, Green, Blue, White, Black, then 5 custom
    float fc[10][3] = {
        {1.0f, 0.0f, 0.0f},  // 0 Red
        {0.0f, 1.0f, 0.0f},  // 1 Green
        {0.0f, 0.0f, 1.0f},  // 2 Blue
        {1.0f, 1.0f, 1.0f},  // 3 White
        {0.0f, 0.0f, 0.0f},  // 4 Black
        {1.0f, 0.5f, 0.0f},  // 5 Orange
        {0.5f, 0.0f, 0.5f},  // 6 Purple
        {0.0f, 1.0f, 1.0f},  // 7 Cyan
        {1.0f, 1.0f, 0.0f},  // 8 Yellow
        {0.5f, 0.5f, 0.5f},  // 9 Grey
    };
    for (int i = 0; i < 10; i++)
        for (int c = 0; c < 3; c++)
            state.floorColors[i][c] = fc[i][c];

    // Ball colours: same 10 entries
    for (int i = 0; i < 10; i++)
        for (int c = 0; c < 3; c++)
            state.ballColors[i][c] = fc[i][c];

    // Light colours: 9 entries — no Black (a black light gives no illumination)
    float lc[9][3] = {
        {1.0f, 0.0f, 0.0f},  // 0 Red
        {0.0f, 1.0f, 0.0f},  // 1 Green
        {0.0f, 0.0f, 1.0f},  // 2 Blue
        {1.0f, 1.0f, 1.0f},  // 3 White
        {1.0f, 0.5f, 0.0f},  // 4 Orange
        {0.5f, 0.0f, 0.5f},  // 5 Purple
        {0.0f, 1.0f, 1.0f},  // 6 Cyan
        {1.0f, 1.0f, 0.0f},  // 7 Yellow
        {0.5f, 0.5f, 0.5f},  // 8 Grey
    };
    for (int i = 0; i < 9; i++)
        for (int c = 0; c < 3; c++)
            state.lightColors[i][c] = lc[i][c];

    state.alpha = 0.5f;

    state.colourTexEnabled      = false;
    state.displacementTexEnabled = false;
    state.alphaTexEnabled        = false;

    state.wireframeEnabled    = false;
    state.lastWireframeToggle = 0.0;

    state.light = createDefaultLight(lightCX, lightCY, lightCZ);

    return state;
}

void resetScene(SceneState& state) {
    state.rotX = 0.0f;
    state.rotY = 0.0f;
    state.rotZ = 0.0f;
    state.sphereStacks = 20;
    state.sphereSectors = 20;
    state.floorResolution = 10;
    state.floorColorIndex = 0;
    state.ballColorIndex = 0;
    state.lightColorIndex = 0;

    // Reset light to default position and color
    resetLight(state.light);
}

void cycleFloorColor(SceneState& state, int direction) {
    state.floorColorIndex = (state.floorColorIndex + direction + 10) % 10;
}

void cycleBallColor(SceneState& state, int direction) {
    state.ballColorIndex = (state.ballColorIndex + direction + 10) % 10;
}

void cycleLightColor(SceneState& state, int direction) {
    state.lightColorIndex = (state.lightColorIndex + direction + 9) % 9;
}

void adjustAlpha(SceneState& state, float delta) {
    state.alpha += delta;
    if (state.alpha < 0.0f) {
        state.alpha = 0.0f;
    } else if (state.alpha > 1.0f) {
        state.alpha = 1.0f;
    }
}
