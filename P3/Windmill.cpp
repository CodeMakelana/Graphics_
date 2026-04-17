#include "Windmill.h"

void Windmill::build() {
    // Red frustum tower and legs for ball gap
    body = buildFrustum(0.8f, 1.2f, 0.8f, 0.5f,  0.8f, 0.1f, 0.1f);
    legLeft = buildCuboid(0.2f, 0.4f, 0.8f,      0.8f, 0.1f, 0.1f);
    legRight = buildCuboid(0.2f, 0.4f, 0.8f,     0.8f, 0.1f, 0.1f);
    roof = buildTriangularPrism(0.8f, 0.5f, 0.4f, 0.6f, 0.0f, 0.0f);
    
    // Metallic gray cylinder axle (>= 8 vertices)
    axle = buildCylinder(0.04f, 0.3f, 12,        0.7f, 0.7f, 0.7f);
    
    // White cuboid blades
    for(int i=0; i<4; i++) {
        bladeStems[i] = buildCuboid(0.04f, 0.4f, 0.02f, 0.9f, 0.9f, 0.8f); 
        bladeStems[i].upload();
        
        // The wider white paddle at the end
        bladeSails[i] = buildCuboid(0.18f, 0.50f, 0.02f, 1.0f, 1.0f, 1.0f);
        bladeSails[i].upload();
    }

    body.upload();
    legLeft.upload();
    legRight.upload();
    roof.upload();
    axle.upload();

    // Windows: one on each side face of the tower body
    windowLeft  = buildCuboid(0.14f, 0.14f, 0.02f, 0.55f, 0.75f, 0.90f);
    windowRight = buildCuboid(0.14f, 0.14f, 0.02f, 0.55f, 0.75f, 0.90f);

    windowLeft.upload();
    windowRight.upload();

    bladeAngle = 0.0f;
    bladeSpeed = 0.0f;
    localTransform = Matrix4::translate(0.0f, 0.0f, 0.0f).multiply(Matrix4::rotateY(270.0f));
}

void Windmill::update(float dt) {
    bladeAngle += bladeSpeed * dt;
    if (bladeAngle >= 360.0f) bladeAngle -= 360.0f;
}

void Windmill::draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe) {
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    Matrix4 base = sceneTransform.multiply(localTransform);

    // Tower Structure
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, base.multiply(Matrix4::translate(0, 0.8f, 0)).getData());
    body.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, base.multiply(Matrix4::translate(-0.3f, 0.2f, 0)).getData());
    legLeft.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, base.multiply(Matrix4::translate(0.3f, 0.2f, 0)).getData());
    legRight.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, base.multiply(Matrix4::translate(0, 1.4f, -0.2f)).getData());
    roof.draw(wireframe);

    // One window on each side face of the tower body (local +X and -X)
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE,
        base.multiply(Matrix4::translate(0.27f, 1.05f, 0.0f)).multiply(Matrix4::rotateY(90.0f)).getData());
    windowLeft.draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE,
        base.multiply(Matrix4::translate(-0.27f, 1.05f, 0.0f)).multiply(Matrix4::rotateY(90.0f)).getData());
    windowRight.draw(wireframe);

    // Animated Rotor Assembly
    Matrix4 rotorBase = base.multiply(Matrix4::translate(0, 1.0f, 0.35f));
    
    // Axle
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, rotorBase.multiply(Matrix4::rotateX(90)).getData());
    axle.draw(wireframe);

    // Blades (Rotating around Local Z)
    for(int i=0; i<4; i++) {
        Matrix4 rot = Matrix4::rotateZ(bladeAngle + (i * 90.0f));
        
        // Draw the narrow stem (closer to the center)
        Matrix4 mStem = rotorBase.multiply(rot).multiply(Matrix4::translate(0.0f, 0.2f, 0.15f));
        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, mStem.getData());
        bladeStems[i].draw(wireframe);

        // Draw the wider sail: shifted in +X so the stem sits on its left edge
        // Y translation keeps a small overlap with the 0.4f stem
        Matrix4 mSail = rotorBase.multiply(rot).multiply(Matrix4::translate(0.1f, 0.58f, 0.16f)); 
        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, mSail.getData());
        bladeSails[i].draw(wireframe);
    }
}