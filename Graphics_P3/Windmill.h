#ifndef WINDMILL_H
#define WINDMILL_H

#include <GL/glew.h>
#include "Shape.h"
#include "MathUtils.h"
#include "ShapeBuilder.h"

struct Windmill {
    // Tower and support legs
    Shape body;
    Shape legLeft;
    Shape legRight;

    // Triangular prism roof
    Shape roof;

    // Arch frame around ball gap
    Shape archLeft;
    Shape archRight;
    Shape archTop;

    // Rotor
    Shape axle;
    Shape blades[4];

    // Blade animation state
    float   bladeAngle;
    float   bladeSpeed;
    Vector3 bladeAxis;

    // Position/orientation of the whole windmill in the scene
    Matrix4 localTransform;

    // Create all shapes and call upload() on each
    void build();

    // Advance blade rotation: bladeAngle += bladeSpeed * dt, wrap at 360
    void update(float dt);

    // Send geometry to the GPU
    void draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe);
};

#endif // WINDMILL_H
