#ifndef SCENE_H
#define SCENE_H

#include "light.h"

struct SceneState {
    float rotX, rotY, rotZ;
    int sphereStacks, sphereSectors;
    int floorResolution;
    int floorColorIndex, ballColorIndex, lightColorIndex;
    float floorColors[10][3];
    float ballColors[10][3];
    float lightColors[9][3];
    float alpha;
    bool colourTexEnabled, displacementTexEnabled, alphaTexEnabled;
    bool wireframeEnabled;
    double lastWireframeToggle;
    PointLight light;
};

SceneState createInitialState(float lightCX, float lightCY, float lightCZ);
void resetScene(SceneState& state);
void cycleFloorColor(SceneState& state, int direction);
void cycleBallColor(SceneState& state, int direction);
void cycleLightColor(SceneState& state, int direction);
void adjustAlpha(SceneState& state, float delta);

#endif