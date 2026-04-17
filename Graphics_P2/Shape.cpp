#include "Shape.h"
#include <cmath>

/**
 * Shape.cpp
 * Base class implementation — GPU upload, draw calls, transform math.
 */

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

Shape::Shape(float cx, float cy, const Vec3& colour)
    : translateX(0.0f), translateY(0.0f)
    , rotation(0.0f)
    , scaleX(1.0f), scaleY(1.0f)
    , centreX(cx), centreY(cy)
    , baseColour(colour)
    , pastelColour(computePastel(colour))
    , selected(false)
    , vao(0), vbo(0)
    , wireVao(0), wireVbo(0)
{}

Shape::~Shape()
{
    // Release GPU resources if they were allocated
    if (vao != 0)     glDeleteVertexArrays(1, &vao);
    if (vbo != 0)     glDeleteBuffers(1, &vbo);
    if (wireVao != 0) glDeleteVertexArrays(1, &wireVao);
    if (wireVbo != 0) glDeleteBuffers(1, &wireVbo);
}

// ─────────────────────────────────────────────────────────────────────────────
//  GPU upload
// ─────────────────────────────────────────────────────────────────────────────

void Shape::init()
{
    // Ask the subclass to fill 'vertices' and 'wireVertices'
    buildVertices();
    buildWireframeVertices();

    // Upload both sets to the GPU
    uploadToGPU(vao,     vbo,     vertices);
    uploadToGPU(wireVao, wireVbo, wireVertices);
}

void Shape::uploadToGPU(GLuint& outVao, GLuint& outVbo,
                        const std::vector<float>& data)
{
    // 1. Create and bind a VAO — records everything below
    glGenVertexArrays(1, &outVao);
    glBindVertexArray(outVao);

    // 2. Create a VBO, bind it, and upload the vertex data
    glGenBuffers(1, &outVbo);
    glBindBuffer(GL_ARRAY_BUFFER, outVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(float), // size in bytes
                 data.data(),                  // pointer to CPU data
                 GL_STATIC_DRAW);              // data won't change after upload

    // 3. Tell OpenGL how to interpret the buffer:
    //    location 0 → vec3 (3 floats), tightly packed, no offset
    glVertexAttribPointer(
        0,          // attribute location (matches "layout(location=0)" in shader)
        3,          // number of components per vertex (x, y, z)
        GL_FLOAT,   // data type
        GL_FALSE,   // don't normalise
        3 * sizeof(float), // stride: bytes between consecutive vertices
        (void*)0    // offset to first component
    );
    glEnableVertexAttribArray(0); // enable location 0

    // 4. Unbind — good practice to avoid accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Draw calls
// ─────────────────────────────────────────────────────────────────────────────

void Shape::draw() const
{
    // Vertex count = total floats / 3 components per vertex
    GLsizei vertexCount = static_cast<GLsizei>(vertices.size() / 3);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Shape::drawWireframe() const
{
    GLsizei vertexCount = static_cast<GLsizei>(wireVertices.size() / 3);

    glBindVertexArray(wireVao);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Transform helpers
// ─────────────────────────────────────────────────────────────────────────────

Mat4 Shape::getTransform() const
{
    // The current world-space centre accounts for any translation applied so far
    float worldCX = centreX + translateX;
    float worldCY = centreY + translateY;

    // Build each component matrix
    Mat4 T  = Mat4::translate(translateX, translateY);
    Mat4 R  = Mat4::rotateAround(rotation, worldCX, worldCY);
    Mat4 S  = Mat4::scaleAround(scaleX, scaleY, worldCX, worldCY);

    // Combine: Translation is already baked into R and S via worldCX/worldCY,
    // so the final matrix is R * S * T (scale first, then rotate, then translate)
    // We apply T first to move centre to world position, then scale and rotate there.
    return R * S * T;
}

Vec3 Shape::getDrawColour() const
{
    return selected ? pastelColour : baseColour;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Private helpers
// ─────────────────────────────────────────────────────────────────────────────

Vec3 Shape::computePastel(const Vec3& colour)
{
    // The spec figures show the selected object turns yellow.
    // Blend the original colour 50% towards yellow (1.0, 1.0, 0.3).
    // This is visible on ALL colours including white, matching the spec examples.
    const Vec3  yellow(1.0f, 1.0f, 0.3f);
    const float blend = 0.5f;
    return Vec3(
        colour.x + (yellow.x - colour.x) * blend,
        colour.y + (yellow.y - colour.y) * blend,
        colour.z + (yellow.z - colour.z) * blend
    );
}