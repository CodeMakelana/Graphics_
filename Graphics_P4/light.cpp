#include "light.h"

PointLight createDefaultLight(float cx, float cy, float cz) {
    PointLight light;

    //Position of light
    light.position = Vector3(cx, cy, cz);
    light.initialPosition = Vector3(cx, cy, cz);

    //Colour of light
    light.r = 1.0f;
    light.g = 1.0f;
    light.b = 1.0f;

    //Intensity of light    
    light.intensity = 2.0f;

    return light;
}

void translateLight(PointLight& light, float dx, float dy, float dz) {
    light.position = light.position.add(Vector3(dx, dy, dz));
}

void setLightColour(PointLight& light, float r, float g, float b) {
    light.r = r;
    light.g = g;
    light.b = b;
}

void resetLight(PointLight& light) {
    light.position = light.initialPosition;
    light.r = light.b = light.g = 1.0f;
    light.intensity = 2.0f;
}

//to be moved to Floor Fragment shader to avoid recalculation of light colour for each fragment on the floor
Vector3 calculatePointLIght( PointLight light, Vector3 surfacePoint, Vector3 surfaceNormal, Vector3 materialColour) {
    Vector3 x = surfacePoint;
    Vector3 r_vec = light.position.subtract(x);
    float r = r_vec.length();
    Vector3 l = r_vec.normalize(); //Light direction vector (unit vector)
    
    //compute light color as Vector3. This is i from the pseudo code
    Vector3 I = Vector3(light.r * light.intensity, light.g * light.intensity, light.b * light.intensity);

    float geom = fmax(0, surfaceNormal.dot(l)) / (r * r);

    Vector3 E = I.scale(geom); //E = I * (n dot l) / r^2

    Vector3 k = materialColour;
    Vector3 kE = Vector3(k.getX() * E.getX(), k.getY() * E.getY(), k.getZ() * E.getZ());
    return kE;
}