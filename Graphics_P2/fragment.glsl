#version 330 core

// ─── Uniforms ─────────────────────────────────────────────────────────────────
// RGB colour for this shape — same for every pixel of a single draw call.
// Set from C++ via glUniform3f before each draw call.
uniform vec3 colour;

// ─── Output ───────────────────────────────────────────────────────────────────
// Final pixel colour written to the framebuffer.
out vec4 fragColour;

// ─────────────────────────────────────────────────────────────────────────────
void main()
{
    // Expand RGB to RGBA — alpha is always 1.0 (fully opaque).
    // No transparency needed for this practical.
    fragColour = vec4(colour, 1.0);
}
