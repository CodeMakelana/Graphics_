#version 330 core

// ---------------------------------------------------------------------------
// sphere.vert
// Vertex shader for the golf ball.
//
// Responsibilities:
//   1. Optionally displace vertices inward along their normals using the
//      displacement texture (creates real geometric dimples, not bump mapping).
//   2. Transform the displaced position through model, view, and projection.
//   3. Pass UV and world-space normal to the fragment shader.
// ---------------------------------------------------------------------------

// Vertex attributes (must match uploadSphereToGPU attribute layout)
layout(location = 0) in vec3 aPos;    // local-space position
layout(location = 1) in vec3 aNormal; // local-space unit normal
layout(location = 2) in vec2 aUV;     // texture coordinate

// Transformation matrices
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// Displacement controls
uniform sampler2D uDisplacementTex;  // greyscale: bright=flat, dark=dimple
uniform bool      uDisplacementEnabled;
uniform float     uDisplacementStrength; // max inward displacement in world units

// Outputs to fragment shader
out vec2 vUV;
out vec3 vNormal;

void main()
{
    vec3 pos = aPos;

    // ---- Displacement mapping (spec 3.2, N key) ----
    // Only applied when the displacement texture is toggled on.
    // Samples the red channel: 0 (dark) = inside dimple = maximum recession,
    // 1 (bright) = flat surface = no movement.
    // Invert so dark regions produce the most inward displacement.
    // Subtract along the normal to recess (adding would create bumps).
    if (uDisplacementEnabled)
    {
        float dispSample = texture(uDisplacementTex, aUV).r;
        float dispAmount = (1.0 - dispSample) * uDisplacementStrength;
        pos = pos - aNormal * dispAmount;
    }

    // ---- Transform to clip space ----
    vec4 worldPos   = uModel * vec4(pos, 1.0);
    gl_Position     = uProjection * uView * worldPos;

    // Pass the original (undisplaced) normal transformed into world space.
    // w=0.0 means translations in uModel do not affect the direction vector.
    vNormal = (uModel * vec4(aNormal, 0.0)).xyz;
    vUV     = aUV;
}
