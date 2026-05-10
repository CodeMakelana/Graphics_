#version 330 core

//inputs form vertex shader
in vec2 vUV;
in vec3 vNormal;

//Uniforms
uniform sampler2D uColourTex;
uniform sampler2D uAlphaTex;

uniform bool uColourTexEnabled;
uniform bool uAlphaTexEnabled;

uniform vec4 uBallColour;   // RGB is the base colour, A is the uniform alpha

//Output
out vec4 FragColour;

void main(){
    vec3 colour = uBallColour.rgb;
    if (uColourTexEnabled) {
        vec3 texColour = texture(uColourTex, vUV).rgb;
        colour = colour * texColour;
    }

    float alpha = uBallColour.a;
    if (uAlphaTexEnabled) {
        float texAlpha = texture(uAlphaTex, vUV).r;
        alpha = alpha * texAlpha;
    }

    FragColour = vec4(colour, alpha);
}