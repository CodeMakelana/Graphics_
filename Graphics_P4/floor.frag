#version 330 core

//inputs from vertex shader
in vec3 vWorldPos;
in vec3 vWorldNormal;
in vec2 vUV;

//Uniforms
uniform vec3 uLightPos;       // p in the pseudocode
uniform vec3 uLightColour;    // feeds into i
uniform float uLightIntensity; // scales i

uniform vec3 uFloorColour;    // base floor colour from the 10-colour cycle
uniform vec4 uBallColour;     // ball RGBA, used to tint the light

//outputs
out vec4 FragColour;

void main() {
    vec3 x = vWorldPos;
    vec3 n = normalize(vWorldNormal);
    vec3 p = uLightPos;

    vec3 pMinusX = p - x;
    float r = max(length(pMinusX), 0.01);
    vec3 l = pMinusX / r;

    vec3 i = uLightColour * uLightIntensity;

    float geom = max(0.0, dot(n, l)) / (r * r);
    vec3 E = i * geom;

    vec3 k = uFloorColour * uBallColour.rgb;

    vec3 ambient = uFloorColour * 0.15;
    vec3 finalColour = ambient + k * E;
    FragColour = vec4(finalColour, 1.0);
}