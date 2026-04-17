#include "Course.h"

void Course::build() {
    grassSurface = buildCuboid(6.0, 0.1, 4.0,   0.10,0.60,0.10);
    grassSurface.upload();

    wallNorth = buildCuboid(6.0, 0.4, 0.1,   0.50,0.30,0.10);
    wallNorth.upload();
    wallSouth = buildCuboid(6.0, 0.4, 0.1,   0.50,0.30,0.10);
    wallSouth.upload();

    wallEast = buildCuboid(0.1, 0.4, 4.0,  0.50,0.30,0.10);
    wallEast.upload();
    wallWest = buildCuboid(0.1, 0.4, 4.0,  0.50,0.30,0.10);
    wallWest.upload();

    startingMat = buildCuboid(0.8, 0.02, 0.5,   0.80,0.40,0.10);
    startingMat.upload();
    holeCylinder = buildCylinder(0.15, 0.12, 12,  0.05,0.05,0.05);
    holeCylinder.upload();
    holeRim = buildCylinder(0.18, 0.03, 12,  0.70,0.70,0.70);
    holeRim.upload();


    plant1Stem = buildCylinder(0.2, 0.8, 8, 0.50,0.30,0.10);
    plant1Stem.upload();
    plant2Stem = buildCylinder(0.2, 0.8, 8, 0.50,0.30,0.10);
    plant2Stem.upload();
    plant3Stem = buildCylinder(0.2, 0.8, 8, 0.50,0.30,0.10);
    plant3Stem.upload();

    plant1Cone = buildCone(0.4, 0.5, 8, 0.10,0.60,0.10);
    plant1Cone.upload();
    plant2Cone = buildCone(0.4, 0.5, 8, 0.10,0.60,0.10);
    plant2Cone.upload();
    plant3Cone = buildCone(0.4, 0.5, 8, 0.10,0.60,0.10);
    plant3Cone.upload();

    flagPole = buildCylinder(0.02, 0.8, 8,    0.90,0.90,0.90);
    flagPole.upload();
    flagBanner = buildCuboid(0.2, 0.1, 0.02,    0.90,0.10,0.10); 
    flagBanner.upload();

}

void Course::draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe) {
    GLint modelLoc = glGetUniformLocation(shaderID, "model");

    //grass surface
    Matrix4 model = sceneTransform.multiply(Matrix4::translate(0, -0.05, 0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    grassSurface.draw(wireframe);

    //walls
    model = sceneTransform.multiply(Matrix4::translate(0.0, 0.2, -2.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    wallNorth.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(0.0, 0.2, 2.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    wallSouth.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(3.0f, 0.2f, 0.0f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    wallEast.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(-3.0f, 0.2f, 0.0f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    wallWest.draw(wireframe);

    //starting mat
    model = sceneTransform.multiply(Matrix4::translate(0.0, 0.01, 1.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    startingMat.draw(wireframe);

    //hole
    model = sceneTransform.multiply(Matrix4::translate(0.0, -0.03, -1.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    holeCylinder.draw(wireframe);
    
    model = sceneTransform.multiply(Matrix4::translate(0.0, 0.015, -1.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    holeRim.draw(wireframe);

    //Plants
    model = sceneTransform.multiply(Matrix4::translate(-3.5, 0.4, 0.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant1Stem.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(-3.5, 0.4, 0.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant2Stem.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(-3.5, 0.4, 0.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant3Stem.draw(wireframe);

    //Plant cones
    model = sceneTransform.multiply(Matrix4::translate(-3.5, 0.8, 0.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant1Cone.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(3.5, 0.8, -1.0));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant2Cone.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(0.0, 0.8, 2.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    plant3Cone.draw(wireframe);


    //Flags
    model = sceneTransform.multiply(Matrix4::translate(0.15, 0.4, -1.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    flagPole.draw(wireframe);

    model = sceneTransform.multiply(Matrix4::translate(0.25, 0.75, -1.5));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    flagBanner.draw(wireframe);}