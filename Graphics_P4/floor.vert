#version 330 core

//inputs form the VBO
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

//Unifroms
uniform mat4 uModel;        //scene rotation Matrix
uniform mat4 uView;         //the lookAt function output
unifomr mat4 uProjection;    //perspective output

//outputs
out vec3 vWorldPos;         //World space Position
out vec3 vWorldNormal;      //World space Normal
out vec2 vUV;               //Kept same since floor ahs no texture

void main() {
    vec4 worldPos4 = uModel * vec4(aPos, 1.0);

    vWorldPos = worldPos4.xyz;
    //vWorldPos = worldPos4.y;
    //vWorldPos = worldPos4.z;

    vWorldNormal = (uModel * vWorldNormal, 0, 0).xyz;

    gl_Position = uProjection * uView * worldPos4;
    vUV = aUV;
}