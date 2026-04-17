#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "Shape.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Circle.h"

/**
 * Scene.hpp
 * Owns all shapes in the golf course layout.
 *
 * Responsibilities:
 *   - Creates and initialises every shape
 *   - Exposes named pointers to the four selectable objects
 *     (golf ball, obstacle1, obstacle2, golf hole)
 *   - Provides drawAll() to render every shape in order
 *   - Manages selection state (only one object selected at a time)
 */
class Scene
{
public:
    // ── Selectable objects (the four key-bound shapes) ────────────────────────
    // Raw pointers into the shapes vector — do NOT delete these directly.
    Shape* golfBall;     // key 1 — white circle  (50+ segments)
    Shape* obstacle1;    // key 2 — triangle obstacle type
    Shape* obstacle2;    // key 3 — low-poly circle obstacle (6-10 segments)
    Shape* golfHole;     // key 4 — pink circle   (50+ segments)

    Scene();
    ~Scene();

    // ── Initialise — uploads all shapes to GPU ────────────────────────────────
    // Must be called once after OpenGL context is ready.
    void init();

    // ── Draw all shapes ───────────────────────────────────────────────────────
    void drawAll(bool wireframeMode) const;

    // ── Access all shapes (used by main render loop) ──────────────────────────
    const std::vector<Shape*>& getShapes() const { return shapes; }

    // ── Selection management ──────────────────────────────────────────────────
    void selectBall();       // key 1
    void selectObstacle1();  // key 2
    void selectObstacle2();  // key 3
    void selectHole();       // key 4
    void deselectAll();      // key 0

private:
    // All shapes in draw order (back to front — painter's algorithm)
    std::vector<Shape*> shapes;

    // Currently selected shape (nullptr = nothing selected)
    Shape* selectedShape;

    // ── Helper: add a shape to the scene and return its pointer ───────────────
    // Takes ownership — shapes are deleted in the destructor.
    template<typename T>
    T* add(T* shape)
    {
        shapes.push_back(shape);
        return shape;
    }

    // ── Sets the selected shape, clears previous selection ───────────────────
    void setSelected(Shape* shape);
};

#endif // SCENE_HPP