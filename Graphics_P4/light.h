#ifndef LIGHT_H
#define LIGHT_H

#include "MathUtils.h"

struct PointLight {
    // Position of the light
    Vector3 position;
    Vector3 initialPosition;
    float r, g, b; // Colour of the light 
    float intensity;
};

PointLight createDefaultLight(float cx, float cy, float cz);
void translateLight(PointLight& light, float dx, float dy, float dz);
void setLightColour(PointLight& light, float r, float g, float b);
void resetLight(PointLight& light);
Vector3 calculatePointLIght( PointLight light, Vector3 surfacePoint, Vector3 surfaceNormal, Vector3 materialColour);

#endif