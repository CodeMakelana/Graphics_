#include "ShapeBuilder.h"

Shape buildCuboid(float w, float h, float d, float r, float g, float b) {
    Shape cuboid;
    float hw = w/2.0f, hh = h/2.0f, hd = d/2.0f;
    cuboid.vertices = {
        -hw,-hh,-hd, r,g,b,  hw,-hh,-hd, r,g,b,  hw, hh,-hd, r,g,b, -hw, hh,-hd, r,g,b,
        -hw,-hh, hd, r,g,b,  hw,-hh, hd, r,g,b,  hw, hh, hd, r,g,b, -hw, hh, hd, r,g,b
    };
    cuboid.indices = { 4,5,6, 4,6,7,  1,0,3, 1,3,2,  0,4,7, 0,7,3,  5,1,2, 5,2,6,  0,1,5, 0,5,4,  3,7,6, 3,6,2 };
    cuboid.wireIndices = { 0,1, 1,5, 5,4, 4,0,  3,2, 2,6, 6,7, 7,3,  0,3, 1,2, 5,6, 4,7 };
    return cuboid;
}

Shape buildFrustum(float w, float h, float d, float topScale, float r, float g, float b) {
    Shape f;
    float tw = (w * topScale) / 2.0f;
    float td = (d * topScale) / 2.0f;
    float hw = w/2.0f, hh = h/2.0f, hd = d/2.0f;

    f.vertices = {
        -hw,-hh,-hd, r,g,b,  hw,-hh,-hd, r,g,b,  hw,-hh, hd, r,g,b, -hw,-hh, hd, r,g,b, // Base
        -tw, hh,-td, r,g,b,  tw, hh,-td, r,g,b,  tw, hh, td, r,g,b, -tw, hh, td, r,g,b  // Top
    };

    f.indices = {
        0,1,2, 0,2,3,  4,6,5, 4,7,6, // Bottom, Top
        0,4,5, 0,5,1,  1,5,6, 1,6,2, // Front, Right
        2,6,7, 2,7,3,  3,7,4, 3,4,0  // Back, Left
    };

    f.wireIndices = { 0,1, 1,2, 2,3, 3,0,  4,5, 5,6, 6,7, 7,4,  0,4, 1,5, 2,6, 3,7 };
    return f;
}

Shape buildTriangularPrism(float base, float height, float depth, float r, float g, float b) {
    Shape TP;
    float hb = base/2.0f;
    TP.vertices = {
        -hb, 0.0f, 0.0f, r,g,b,   hb, 0.0f, 0.0f, r,g,b,  0.0f, height, 0.0f, r,g,b,
        -hb, 0.0f, depth, r,g,b,  hb, 0.0f, depth, r,g,b, 0.0f, height, depth, r,g,b
    };
    TP.indices = { 0,1,2,  3,5,4,  0,3,4, 0,4,1,  2,5,3, 2,3,0,  1,4,5, 1,5,2 };
    TP.wireIndices = { 0,1, 1,2, 2,0,  3,4, 4,5, 5,3,  0,3, 1,4, 2,5 };
    return TP;
}

Shape buildCylinder(float radius, float height, int segments, float r, float g, float b) {
    Shape cyl;
    float pi = 3.14159265358979323846f;
    
    // Bottom and Top rings
    for (int i = 0; i < segments; i++) {
        float angle = 2*pi*i / segments;
        float x = radius * cos(angle), z = radius * sin(angle);
        cyl.vertices.insert(cyl.vertices.end(), {x, -height/2.0f, z, r, g, b});
    }
    for (int i = 0; i < segments; i++) {
        float angle = 2*pi*i / segments;
        float x = radius * cos(angle), z = radius * sin(angle);
        cyl.vertices.insert(cyl.vertices.end(), {x, height/2.0f, z, r, g, b});
    }
    
    // Centers
    cyl.vertices.insert(cyl.vertices.end(), {0, -height/2.0f, 0, r, g, b});
    cyl.vertices.insert(cyl.vertices.end(), {0, height/2.0f, 0, r, g, b});

    int bCen = 2 * segments, tCen = 2 * segments + 1;
    for (int i = 0; i < segments; i++) {
        int next = (i+1) % segments;
        // Sides
        cyl.indices.insert(cyl.indices.end(), {(unsigned int)i, (unsigned int)next, (unsigned int)(segments+next)});
        cyl.indices.insert(cyl.indices.end(), {(unsigned int)i, (unsigned int)(segments+next), (unsigned int)(segments+i)});
        // Caps
        cyl.indices.insert(cyl.indices.end(), {(unsigned int)bCen, (unsigned int)next, (unsigned int)i});
        cyl.indices.insert(cyl.indices.end(), {(unsigned int)tCen, (unsigned int)(segments+i), (unsigned int)(segments+next)});
        // Wireframe
        cyl.wireIndices.insert(cyl.wireIndices.end(), {(unsigned int)i, (unsigned int)next, (unsigned int)(segments+i), (unsigned int)(segments+next), (unsigned int)i, (unsigned int)(segments+i)});
    }
    return cyl;
}

Shape buildCone(float radius, float height, int segments, float r, float g, float b) {
    Shape cone;
    float pi = 3.14159265358979323846f;
    
    // Base ring
    for (int i = 0; i < segments; i++) {
        float angle = 2*pi*i / segments;
        cone.vertices.insert(cone.vertices.end(), {
            radius * cosf(angle), 0.0f, radius * sinf(angle), r, g, b}
        );
    }
    // Base center and Apex
    cone.vertices.insert(cone.vertices.end(), {0, 0, 0, r, g, b});
    cone.vertices.insert(cone.vertices.end(), {0, height, 0, r, g, b});

    int bCen = segments, apex = segments + 1;
    for (int i = 0; i < segments; i++) {
        int next = (i+1) % segments;
        cone.indices.insert(cone.indices.end(), {(unsigned int)bCen, (unsigned int)next, (unsigned int)i});
        cone.indices.insert(cone.indices.end(), {(unsigned int)apex, (unsigned int)i, (unsigned int)next});
        cone.wireIndices.insert(cone.wireIndices.end(), {(unsigned int)i, (unsigned int)next, (unsigned int)apex, (unsigned int)i});
    }
    return cone;
}

Shape buildSphere(float radius, int stacks, int slices, float r, float g, float b) {
    Shape sphere;
    float pi = 3.14159265358979323846f;

    for (int i = 0; i <= stacks; i++) {
        float v = (float)i / (float)stacks;
        float phi = pi * v;
        float y = radius * cosf(phi);
        float ring = radius * sinf(phi);

        for (int j = 0; j <= slices; j++) {
            float u = (float)j / (float)slices;
            float theta = 2.0f * pi * u;
            float x = ring * cosf(theta);
            float z = ring * sinf(theta);
            sphere.vertices.insert(sphere.vertices.end(), {x, y, z, r, g, b});
        }
    }

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            unsigned int a = (unsigned int)(i * (slices + 1) + j);
            unsigned int b = a + (unsigned int)(slices + 1);
            unsigned int c = a + 1;
            unsigned int d = b + 1;

            sphere.indices.insert(sphere.indices.end(), {a, b, c});
            sphere.indices.insert(sphere.indices.end(), {c, b, d});

            sphere.wireIndices.insert(sphere.wireIndices.end(), {a, b, a, c});
        }
    }

    return sphere;
}