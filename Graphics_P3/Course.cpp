#include "Course.h"

void Course::build() {
    grassSurface = buildCuboid(6.0f, 0.01f, 4.0f,  0.2f, 0.6f, 0.2f); // Green
    
    wallN = buildCuboid(6.2f, 0.4f, 0.1f,         0.4f, 0.3f, 0.2f); // Brown
    wallS = buildCuboid(6.2f, 0.4f, 0.1f,         0.4f, 0.3f, 0.2f);
    wallE = buildCuboid(0.1f, 0.4f, 4.0f,         0.4f, 0.3f, 0.2f);
    wallW = buildCuboid(0.1f, 0.4f, 4.0f,         0.4f, 0.3f, 0.2f);

    startingMat = buildCuboid(0.8f, 0.02f, 0.5f,  0.9f, 0.3f, 0.1f); // Distinct Red/Orange
    startBall = buildSphere(0.05f, 12, 16, 0.95f, 0.95f, 0.95f);
    holeCylinder = buildCylinder(0.15f, 0.05f, 16, 0.1f, 0.1f, 0.1f); // Dark 

    for(int i=0; i<2; i++) {
        plantStem[i] = buildCylinder(0.05f, 0.4f, 8, 0.3f, 0.2f, 0.1f);
        plantLeaves[i] = buildCone(0.3f, 0.6f, 10,   0.1f, 0.5f, 0.1f);
        plantStem[i].upload();
        plantLeaves[i].upload();
    }

    vObstacleLeft = buildCuboid(0.8f, 0.05f, 0.05f, 0.9f, 0.9f, 0.9f); 
    vObstacleRight = buildCuboid(0.8f, 0.05f, 0.05f, 0.9f, 0.9f, 0.9f);

    grassSurface.upload(); wallN.upload(); wallS.upload(); wallE.upload(); wallW.upload();
    startingMat.upload(); startBall.upload(); holeCylinder.upload();
    vObstacleLeft.upload(); vObstacleRight.upload();
}

void Course::draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe) {
    GLint modelLoc = glGetUniformLocation(shaderID, "model");

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(0, -0.005f, 0)).getData());
    grassSurface.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(0, 0.2f, -2.05f)).getData());
    wallN.draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(0, 0.2f, 2.05f)).getData());
    wallS.draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(3.05f, 0.2f, 0)).getData());
    wallE.draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(-3.05f, 0.2f, 0)).getData());
    wallW.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(-2.0f, 0.01f, 0)).getData());
    startingMat.draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(-2.0f, 0.07f, 0)).getData());
    startBall.draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(2.5f, 0.0f, 0.0f)).getData());
    holeCylinder.draw(wireframe);

    // Decor outside boundary
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(-3.5f, 0.2f, 1.5f)).getData());
    plantStem[0].draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(-3.5f, 0.4f, 1.5f)).getData());
    plantLeaves[0].draw(wireframe);

    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(3.5f, 0.2f, -1.5f)).getData());
    plantStem[1].draw(wireframe);
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, sceneTransform.multiply(Matrix4::translate(3.5f, 0.4f, -1.5f)).getData());
    plantLeaves[1].draw(wireframe);

        Matrix4 mLeftArm = sceneTransform
            .multiply(Matrix4::translate(-1.2025f, 0.03f, 1.7732f))
        .multiply(Matrix4::rotateY(135.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, mLeftArm.getData());
    vObstacleLeft.draw(wireframe);
 
    Matrix4 mRightArm = sceneTransform
        .multiply(Matrix4::translate(-1.7998f, 0.03f, 1.7732f))
        .multiply(Matrix4::rotateY(-135.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, mRightArm.getData());
    vObstacleRight.draw(wireframe);
}