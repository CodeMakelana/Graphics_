#version 330 core

//inputs (from VBO)
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

//Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform sampler2D uDisplacementTex;
uniform bool uDisplacementEnabled;
uniform float uDisplacementStrength;

//Outputs ro fragment shader
out vec2 vUV;
out vec3 vNormal;

void main() {
    vec3 pos = aPos;
    float dispSample = texture(uDisplacementTex, aUV).r;
    float dispAmount = (1.0 - dispSample) * uDisplacementStrength;
    pos = pos - aNormal * dispAmount;

    vec4 worldPos = uModel * vec4(pos, 1.0);
    gl_Position = uProjection * uView * worldPos;

    vNormal = (uModel * vec4(aNormal, 0.0)).xyz;
    vUV = aUV;
}