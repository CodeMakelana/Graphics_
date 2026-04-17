#include "Scene.h"

// Mini-golf scene setup.
// Ball starts at the bottom-left tee and the hole is in the top-right.

// Color palette
static const Vec3 COL_FLOOR  (0.52f, 0.52f, 0.52f);
static const Vec3 COL_GRASS  (0.16f, 0.50f, 0.18f);
static const Vec3 COL_WALL   (0.28f, 0.16f, 0.06f);
static const Vec3 COL_RIVER  (0.10f, 0.30f, 0.78f);
static const Vec3 COL_BRIDGE (0.65f, 0.45f, 0.20f);
static const Vec3 COL_TEE    (0.50f, 0.05f, 0.10f);
static const Vec3 COL_OBS1   (0.76f, 0.60f, 0.30f);
static const Vec3 COL_OBS2   (0.78f, 0.12f, 0.12f);
static const Vec3 COL_OBS3   (0.45f, 0.10f, 0.62f);
static const Vec3 COL_HOLE   (0.96f, 0.42f, 0.68f);
static const Vec3 COL_BALL   (0.97f, 0.97f, 0.97f);

// Layout constants
static const float FLOOR_W   = 0.90f; // floor half-width
static const float FLOOR_H   = 0.84f; // floor half-height
static const float GRASS_W   = 0.81f; // grass half-width  (inset from floor)
static const float GRASS_H   = 0.75f; // grass half-height
static const float WALL_T    = 0.030f;// boundary wall half-thickness

// River position and size
static const float RIVER_CX  = -0.08f;
static const float RIVER_HW  =  0.08f;
static const float RIVER_HH  = GRASS_H; // touches both boundary walls

// Bridge dimensions
static const float BRIDGE_CY_TOP = 0.18f;  // upper plank centre Y
static const float BRIDGE_CY_BOT = -0.18f; // lower plank centre Y
static const float BRIDGE_HW     = RIVER_HW + 0.12f; // slightly wider than river
static const float BRIDGE_HH     = 0.055f;            // plank half-height
static const float BRIDGE_RAIL_T = 0.018f;            // rail half-thickness

Scene::Scene()
    : golfBall(nullptr)
    , obstacle1(nullptr)
    , obstacle2(nullptr)
    , golfHole(nullptr)
    , selectedShape(nullptr)
{
    // Concrete floor
    add(new Rectangle(0.0f, 0.0f, FLOOR_W, FLOOR_H, COL_FLOOR));

    // Grass area
    add(new Rectangle(0.0f, 0.0f, GRASS_W, GRASS_H, COL_GRASS));

    // Boundary walls
    add(new Rectangle( 0.00f,  GRASS_H, GRASS_W,  WALL_T, COL_WALL)); // top
    add(new Rectangle( 0.00f, -GRASS_H, GRASS_W,  WALL_T, COL_WALL)); // bottom
    add(new Rectangle(-GRASS_W, 0.00f,  WALL_T,  GRASS_H, COL_WALL)); // left
    add(new Rectangle( GRASS_W, 0.00f,  WALL_T,  GRASS_H, COL_WALL)); // right

    // River
    add(new Rectangle(RIVER_CX, 0.0f, RIVER_HW, RIVER_HH, COL_RIVER));

    // Bridge planks
    add(new Rectangle(RIVER_CX, BRIDGE_CY_TOP, BRIDGE_HW, BRIDGE_HH, COL_BRIDGE));
    add(new Rectangle(RIVER_CX, BRIDGE_CY_BOT, BRIDGE_HW, BRIDGE_HH, COL_BRIDGE));

    // Bridge rails
    add(new Rectangle(RIVER_CX,
                       BRIDGE_CY_TOP + BRIDGE_HH - BRIDGE_RAIL_T,
                       BRIDGE_HW, BRIDGE_RAIL_T, COL_WALL));
    add(new Rectangle(RIVER_CX,
                       BRIDGE_CY_TOP - BRIDGE_HH + BRIDGE_RAIL_T,
                       BRIDGE_HW, BRIDGE_RAIL_T, COL_WALL));
    add(new Rectangle(RIVER_CX,
                       BRIDGE_CY_BOT + BRIDGE_HH - BRIDGE_RAIL_T,
                       BRIDGE_HW, BRIDGE_RAIL_T, COL_WALL));
    add(new Rectangle(RIVER_CX,
                       BRIDGE_CY_BOT - BRIDGE_HH + BRIDGE_RAIL_T,
                       BRIDGE_HW, BRIDGE_RAIL_T, COL_WALL));

    // Tee box
    add(new Rectangle(-0.60f, -0.57f, 0.12f, 0.12f, COL_TEE));

    // Obstacle type 1 (rectangles)
    add(new Rectangle(-0.52f,  0.40f, 0.18f, 0.055f, COL_OBS1)); // left-upper
    add(new Rectangle(-0.52f, -0.15f, 0.15f, 0.055f, COL_OBS1)); // left-lower

    add(new Rectangle( 0.28f,  0.55f, 0.22f, 0.055f, COL_OBS1)); // right-top
    add(new Rectangle( 0.55f,  0.05f, 0.18f, 0.055f, COL_OBS1)); // right-mid
    add(new Rectangle( 0.35f, -0.45f, 0.20f, 0.055f, COL_OBS1)); // right-low

    // Obstacle type 2 (triangles)
    obstacle1 = add(new Triangle(
        -0.70f,  0.05f,   // bottom-left
        -0.54f,  0.05f,   // bottom-right
        -0.62f,  0.25f,   // apex
         COL_OBS2
    ));

    add(new Triangle(
         0.62f, -0.55f,
         0.76f, -0.55f,
         0.69f, -0.35f,
         COL_OBS2
    ));

    // Obstacle type 3 (octagons)
    obstacle2 = add(new Circle(-0.35f,  0.60f, 0.070f, 8, COL_OBS3));

    add(new Circle( 0.68f,  0.30f, 0.065f, 8, COL_OBS3));

    // Golf hole
    golfHole = add(new Circle(0.62f, 0.60f, 0.075f, 60, COL_HOLE));

    // Golf ball
    golfBall = add(new Circle(-0.60f, -0.57f, 0.050f, 60, COL_BALL));
}

Scene::~Scene()
{
    for (Shape* s : shapes)
        delete s;
    shapes.clear();
}

void Scene::init()
{
    for (Shape* s : shapes)
        s->init();
}

void Scene::drawAll(bool wireframeMode) const
{
    for (const Shape* s : shapes)
    {
        if (wireframeMode)
            s->drawWireframe();
        else
            s->draw();
    }
}

// Selection helpers

void Scene::setSelected(Shape* shape)
{
    if (selectedShape != nullptr)
        selectedShape->selected = false;
    selectedShape = shape;
    if (selectedShape != nullptr)
        selectedShape->selected = true;
}

void Scene::selectBall()      { setSelected(golfBall);  }
void Scene::selectObstacle1() { setSelected(obstacle1); }
void Scene::selectObstacle2() { setSelected(obstacle2); }
void Scene::selectHole()      { setSelected(golfHole);  }
void Scene::deselectAll()     { setSelected(nullptr);   }