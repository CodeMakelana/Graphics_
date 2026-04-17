#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <vector>
#include "Vec3.h"
#include "Mat4.h"

/**
 * Shape.hpp
 * Abstract base class for all 2D renderable shapes.
 *
 * Responsibilities:
 *   - Owns the VAO and VBO on the GPU
 *   - Stores the shape's colour and transform state
 *   - Provides draw() for filled mode and drawWireframe() for GL_LINES mode
 *   - Tracks selection state (changes colour to pastel when selected)
 *
 * Subclasses (Rectangle, Triangle, Circle) only need to:
 *   - Fill buildVertices() with their specific vertex positions
 *   - Fill buildWireframeVertices() with their edge vertices
 */
class Shape
{
public:
    // ── Transform state ───────────────────────────────────────────────────────
    // These accumulate as the user presses transform keys.
    float translateX;   // current X offset in NDC
    float translateY;   // current Y offset in NDC
    float rotation;     // current rotation in radians
    float scaleX;       // current X scale factor
    float scaleY;       // current Y scale factor

    // ── Centre point (NDC) ────────────────────────────────────────────────────
    // Used as the pivot for rotation and scale.
    // Set in the subclass constructor to the shape's initial centre.
    float centreX;
    float centreY;

    // ── Colour ────────────────────────────────────────────────────────────────
    Vec3 baseColour;    // original colour
    Vec3 pastelColour;  // highlight colour when selected (computed from base)

    // ── Selection state ───────────────────────────────────────────────────────
    bool selected;

    // ── Constructor / Destructor ──────────────────────────────────────────────
    Shape(float cx, float cy, const Vec3& colour);
    virtual ~Shape();

    // ── GPU upload ────────────────────────────────────────────────────────────
    /**
     * Uploads vertex data to the GPU (creates VAO + VBO).
     * Must be called once after construction, before draw().
     */
    void init();

    // ── Draw calls ────────────────────────────────────────────────────────────
    /** Draw filled shape using GL_TRIANGLES. */
    void draw() const;

    /** Draw wireframe using GL_LINES. */
    void drawWireframe() const;

    // ── Transform helpers ─────────────────────────────────────────────────────
    /**
     * Builds the combined transform matrix from current state.
     * Order: Translation * RotateAround(centre) * ScaleAround(centre)
     */
    Mat4 getTransform() const;

    /**
     * Returns the active draw colour:
     *   - pastelColour if selected
     *   - baseColour   otherwise
     */
    Vec3 getDrawColour() const;

protected:
    // ── GPU object handles ────────────────────────────────────────────────────
    GLuint vao;             // Vertex Array Object  — remembers VBO layout
    GLuint vbo;             // Vertex Buffer Object — stores fill vertices on GPU
    GLuint wireVao;         // VAO for wireframe vertices
    GLuint wireVbo;         // VBO for wireframe vertices

    // ── Vertex data (CPU side) ────────────────────────────────────────────────
    std::vector<float> vertices;          // fill vertices  (x, y, z per vertex)
    std::vector<float> wireVertices;      // edge vertices  (x, y, z per vertex)

    // ── Pure virtual — subclasses provide their geometry ──────────────────────
    virtual void buildVertices()          = 0; // fill 'vertices'
    virtual void buildWireframeVertices() = 0; // fill 'wireVertices'

private:
    /**
     * Uploads a vertex array to a VAO/VBO pair.
     * location 0 = position attribute (vec3).
     */
    void uploadToGPU(GLuint& outVao, GLuint& outVbo,
                     const std::vector<float>& data);

    /**
     * Computes a pastel version of the given colour.
     * Pastel = blend 60% towards white: pastel = colour * 0.4 + white * 0.6
     */
    static Vec3 computePastel(const Vec3& colour);
};

#endif // SHAPE_H