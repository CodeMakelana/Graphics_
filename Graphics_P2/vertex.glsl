#version 330 core

// ─── Input ────────────────────────────────────────────────────────────────────
// Per-vertex position coming from the VBO (location 0).
// Even though our scene is 2D, we use vec3 (x, y, z=0).
layout(location = 0) in vec3 position;

// ─── Uniforms ─────────────────────────────────────────────────────────────────
// Transformation matrix — combines translation, rotation, and scale.
// Set from C++ via glUniformMatrix4fv before each draw call.
uniform mat4 transform;

// ─────────────────────────────────────────────────────────────────────────────
void main()
{
    // Promote the 3D position to homogeneous 4D coordinates (w = 1.0).
    // w = 1.0 marks this as a *position* (not a direction).
    // The transform matrix is then applied — this is where all Mat4 work is used.
    gl_Position = transform * vec4(position, 1.0);
}
