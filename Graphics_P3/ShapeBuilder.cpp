#include "ShapeBuilder.h"

Shape buildCuboid(float w, float h, float d, float r, float g, float b) {
    Shape cuboid;

    //vertex 0
    cuboid.vertices.push_back(-w/2.0f); //x axis
    cuboid.vertices.push_back(-h/2.0f); //y axis
    cuboid.vertices.push_back(-d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue
    
    //vertex 1
    cuboid.vertices.push_back(w/2.0f); //x axis
    cuboid.vertices.push_back(-h/2.0f); //y axis
    cuboid.vertices.push_back(-d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 2
    cuboid.vertices.push_back(w/2.0f); //x axis
    cuboid.vertices.push_back(h/2.0f); //y axis
    cuboid.vertices.push_back(-d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 3
    cuboid.vertices.push_back(-w/2.0f); //x axis
    cuboid.vertices.push_back(h/2.0f); //y axis
    cuboid.vertices.push_back(-d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 4
    cuboid.vertices.push_back(-w/2.0f); //x axis
    cuboid.vertices.push_back(-h/2.0f); //y axis
    cuboid.vertices.push_back(d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 5
    cuboid.vertices.push_back(w/2.0f); //x axis
    cuboid.vertices.push_back(-h/2.0f); //y axis
    cuboid.vertices.push_back(d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 6
    cuboid.vertices.push_back(w/2.0f); //x axis
    cuboid.vertices.push_back(h/2.0f); //y axis
    cuboid.vertices.push_back(d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue

    //vertex 7
    cuboid.vertices.push_back(-w/2.0f); //x axis
    cuboid.vertices.push_back(h/2.0f); //y axis
    cuboid.vertices.push_back(d/2.0f); //z axis
    cuboid.vertices.push_back(r); //red
    cuboid.vertices.push_back(g); //green
    cuboid.vertices.push_back(b); //blue


    cuboid.indices = {
    // Front
    4, 5, 6,   4, 6, 7,

    // Back
    1, 0, 3,   1, 3, 2,

    // Left
    0, 4, 7,   0, 7, 3,

    // Right
    5, 1, 2,   5, 2, 6,

    // Bottom
    0, 1, 5,   0, 5, 4,

    // Top
    3, 7, 6,   3, 6, 2
};

    cuboid.wireIndices = {
        //Bottom
        0,1, 1,5, 5,4, 4,0,
        //top
        3,2, 2,6, 6,7, 7,3,
        //verts
        0,3, 1,2, 5,6, 4,7 
    };


    return cuboid;

}

Shape buildTriangularPrism(float base, float height, float depth, float r, float g, float b) {
    Shape TP;

    //vertex 0
    TP.vertices.push_back(-base/2.0f); //x-axis
    TP.vertices.push_back(0.0f); //y-axis
    TP.vertices.push_back(0.0f); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //vertex 1
    TP.vertices.push_back(base/2.0f); //x-axis
    TP.vertices.push_back(0.0f); //y-axis
    TP.vertices.push_back(0.0f); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //vertex 2
    TP.vertices.push_back(0.0f); //x-axis
    TP.vertices.push_back(height); //y-axis
    TP.vertices.push_back(0.0f); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //vertex 3
    TP.vertices.push_back(-base/2.0f); //x-axis
    TP.vertices.push_back(0.0f); //y-axis
    TP.vertices.push_back(depth); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //vertex 4
    TP.vertices.push_back(base/2.0f); //x-axis
    TP.vertices.push_back(0.0f); //y-axis
    TP.vertices.push_back(depth); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //vertex 5
    TP.vertices.push_back(0.0f); //x-axis
    TP.vertices.push_back(height); //y-axis
    TP.vertices.push_back(depth); //z-axis
    TP.vertices.push_back(r); //red
    TP.vertices.push_back(g); //green
    TP.vertices.push_back(b); //blue

    //Add the 15 triangle indices here (5 faces, 3 triangles per face)
    TP.indices = {
        //front face
        0,1,2,
        //back face
        3,4,5,
        //bottom face
        0,3,4, 0,4,1,
        //left face
        2,5,3, 2,3,0,
        //right face
        1,4,5, 1,5,2
    };

    //Add the 9 wireframe indices here (3 faces, 2 edges per face)
    TP.wireIndices = {
        0,1, 1,2, 2,0,
        3,4, 4,5, 5,3,
        0,3, 1,4, 2,5
    };

    return TP;
}

Shape buildCylinder(float radius, float height, int segments, float r, float g, float b) {
    Shape cylinder;
    float pi = 3.14159265358979323846f;

    int N = segments;

    //bottom circle verts
    for (int i = 0; i < N; i++) {
        float angle = 2*pi*i / N;
        float y = -height/2.0f;
        float x = radius * cos(angle);
        float z= radius * sin(angle);
        cylinder.vertices.push_back(x);
        cylinder.vertices.push_back(y);
        cylinder.vertices.push_back(z);
        cylinder.vertices.push_back(r); //red
        cylinder.vertices.push_back(g); //green
        cylinder.vertices.push_back(b); //blue
    }

    //top circle verts
    for (int i = 0; i < N; i++) {
        float angle = 2*pi*i / N;
        float y = height/2.0f;
        float x = radius * cos(angle);
        float z= radius * sin(angle);
        cylinder.vertices.push_back(x);
        cylinder.vertices.push_back(y);
        cylinder.vertices.push_back(z);
        cylinder.vertices.push_back(r); //red
        cylinder.vertices.push_back(g); //green
        cylinder.vertices.push_back(b); //blue
    }

    //bottom center
    cylinder.vertices.push_back(0);
    cylinder.vertices.push_back(-height/2.0f);
    cylinder.vertices.push_back(0);
    cylinder.vertices.push_back(r); //red
    cylinder.vertices.push_back(g); //green
    cylinder.vertices.push_back(b); //blue

    //top center 
    cylinder.vertices.push_back(0);
    cylinder.vertices.push_back(height/2.0f);
    cylinder.vertices.push_back(0);
    cylinder.vertices.push_back(r); //red
    cylinder.vertices.push_back(g); //green
    cylinder.vertices.push_back(b); //blue

    //Indices
    int bottomCenter = 2 * N;
    int topCenter    = 2 * N + 1;

    for (int i = 0; i < N; i++) {
        float next  = (i+1) % N;
        float afterNext = N + next;

        //side quads
        cylinder.indices.push_back(i);
        cylinder.indices.push_back(next);
        cylinder.indices.push_back(afterNext);

        cylinder.indices.push_back(i);
        cylinder.indices.push_back(afterNext);
        cylinder.indices.push_back(N+i);

        //bottom cap
        cylinder.indices.push_back(bottomCenter);
        cylinder.indices.push_back(next);
        cylinder.indices.push_back(i);

        //top cap
        cylinder.indices.push_back(topCenter);
        cylinder.indices.push_back(N+i);
        cylinder.indices.push_back(afterNext);
    }

    //wire indices
    for (int i = 0; i < N; i++) {
        float next  = (i+1) % N;
        float afterNext = N + next;
        
        //bottom ring
        cylinder.wireIndices.push_back(i);
        cylinder.wireIndices.push_back(next);

        //top ring
        cylinder.wireIndices.push_back(N+i);
        cylinder.wireIndices.push_back(afterNext);

        //verts
        cylinder.wireIndices.push_back(i);
        cylinder.wireIndices.push_back(N+i);
    }

    return cylinder;
}

Shape buildCone(float radius, float height, int segments, float r, float g, float b) {
    Shape cone;

    int N = segments;
    float pi = 3.14159265358979323846f;
    
    //base ring
    for (int i =0; i < N; i++) {
        float angle = 2*pi*i / N;
        float y = 0;
        float x = radius * cos(angle);
        float z= radius * sin(angle);
        cone.vertices.push_back(x);
        cone.vertices.push_back(y);
        cone.vertices.push_back(z);
        cone.vertices.push_back(r); //red
        cone.vertices.push_back(g); //green
        cone.vertices.push_back(b); //blue
    }

    //Base center
    cone.vertices.push_back(0);
    cone.vertices.push_back(0);
    cone.vertices.push_back(0);
    cone.vertices.push_back(r); //red
    cone.vertices.push_back(g); //green
    cone.vertices.push_back(b); //blue

    //apex
    cone.vertices.push_back(0);
    cone.vertices.push_back(height);
    cone.vertices.push_back(0);
    cone.vertices.push_back(r); //red
    cone.vertices.push_back(g); //green
    cone.vertices.push_back(b); //blue

    //Triangle indices
      for (int i = 0; i < N; i++) {
        float next  = (i+1) % N;
        float afterNext = N + next;

        //Base cap
        cone.indices.push_back(N);
        cone.indices.push_back(next);
        cone.indices.push_back(i);

        //side
        cone.indices.push_back(N+i);
        cone.indices.push_back(i);
        cone.indices.push_back(next);
    }

    //wire indices
    for (int i = 0; i < N; i++) {
        float next  = (i+1)%N;
        float afterNext = N + next;
        
        //bottom ring
        cone.wireIndices.push_back(i);
        cone.wireIndices.push_back(next);

        //angled lines
        cone.wireIndices.push_back(N+i);
        cone.wireIndices.push_back(i);
    }
    return cone;
}
