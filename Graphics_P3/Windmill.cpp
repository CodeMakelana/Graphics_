#include "Windmill.h"

void Windmill::build() {
    
    body = buildCuboid(0.6f, 1.6f, 0.6f,  0.80f,0.70f,0.50f);
    legLeft = buildCuboid(0.2f, 0.4f, 0.6f,  0.70f,0.60f,0.45f);
    legRight = buildCuboid(0.2f, 0.4f, 0.6f,  0.70f,0.60f,0.45f);

    roof = buildTriangularPrism(0.7f, 0.4f, 0.7f,  0.60f,0.30f,0.20f);

    archLeft = buildCuboid(0.08f, 0.45f, 0.08f,  0.50f,0.50f,0.50f);
    archRight = buildCuboid(0.08f, 0.45f, 0.08f,  0.50f,0.50f,0.50f);
    archTop = buildCuboid(0.64f, 0.08f, 0.08f,  0.50f,0.50f,0.50f);

    //upload the shapes
    body.upload();
    legLeft.upload();
    legRight.upload();

    roof.upload();

    archLeft.upload();
    archRight.upload();
    archTop.upload();

    for (int i = 0; i < 4; i++) {
        blades[i] = buildCuboid(0.10f, 0.67f, 0.02f,  0.85f,0.85f,0.85f);
        blades[i].upload();
    }

    //initialise animation
    bladeAngle = 0.0f;
    bladeSpeed = 0.0f;
    bladeAxis = Vector3(0.0f, 0.0f, 1.0f);
    localTransform = Matrix4::translate(1.5f, 0.8f, -1.5f);
}

void Windmill::update(float dt) {
    bladeAngle += bladeSpeed * dt;

    if (bladeAngle >= 360.0f) {
        bladeAngle -= 360.0f;
    }
}

void Windmill::draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe) {
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    Matrix4 windmillBase = sceneTransform.multiply(localTransform);

    //Body
    Matrix4 model = windmillBase.multiply(Matrix4::identity());
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model.getData());
    body.draw(wireframe);

    //legLeft
    Matrix4 model1 = windmillBase.multiply(Matrix4::translate(-0.2f, 0.2f, 0.0f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model1.getData());
    legLeft.draw(wireframe);

    //right leg
    Matrix4 model2 = windmillBase.multiply(Matrix4::translate(0.2f, 0.2f, 0.0f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model2.getData());
    legRight.draw(wireframe);

    //roof
    Matrix4 model3 = windmillBase.multiply(Matrix4::translate(0.0f, 0.8f, -0.35f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model3.getData());
    roof.draw(wireframe);

    //left arch
    Matrix4 model4 = windmillBase.multiply(Matrix4::translate(-0.24f, -0.575f, 0.3f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model4.getData());
    archLeft.draw(wireframe);

    //right arch
    Matrix4 model5 = windmillBase.multiply(Matrix4::translate(0.24f, -0.575f, 0.3f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model5.getData());
    archRight.draw(wireframe);

    //top arch
    Matrix4 model6 = windmillBase.multiply(Matrix4::translate(0.0f, -0.35f, 0.3f));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model6.getData());
    archTop.draw(wireframe);

    //axle
    Matrix4 model7 = windmillBase.multiply(Matrix4::translate(0,1.0f,0.5f).multiply(Matrix4::rotateX(90)));
    glUniformMatrix4fv(modelLoc,1,GL_TRUE,model7.getData());
    axle.draw(wireframe);

    //blades
    Matrix4 bladeInner = Matrix4::translate(0.0f, 0.335f, 0.0f);
    Matrix4 axlePos = Matrix4::translate(0.0f, 1.0f, 0.5f);
    for (int i = 0; i < 4; i++) {
        Matrix4 bladeRot = Matrix4::rotateArbitraryAxis(
            Vector3(0.0f, 0.0f, 1.0f),
            bladeAngle + i * 90.0f
        );
        Matrix4 bladeModel = windmillBase.multiply(axlePos)
                                         .multiply(bladeRot)
                                         .multiply(bladeInner);
        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, bladeModel.getData());
        blades[i].draw(wireframe);
    }
}