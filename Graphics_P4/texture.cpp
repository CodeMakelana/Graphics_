#include "texture.h"

GLuint createColourTexture(int width, int height) {
    GLuint textureID;
    int size = width * height * 4;
    unsigned char* pixelData = new unsigned char[size];

    const int tileSize = 32;
    const int radius = 10;
    for (int x = 0; x < width; x++) {       //x pos
        for (int y = 0; y < height; y++) {   //y pos
            float localX = x % tileSize;
            float localY = y % tileSize;

            float centerX = tileSize / 2;
            float centerY = tileSize / 2;
            
            int dist = sqrt(((localX - centerX) * (localX - centerX)) + ((localY - centerY) * (localY - centerY)));

            int index = (y * width + x) * 4; // index for the pixel array


            if (dist < radius) { // a dimple.
                //write a darker colour to the pixel data 
                pixelData[index] = 0;       //red
                pixelData[index + 1] = 0;   //green
                pixelData[index + 2] = 0;   //blue
                pixelData[index + 3] = 255; // alpha channel
            } else { // a flat area
                //write a lighter colour to the pixel data 
                pixelData[index] = 255;     //red
                pixelData[index + 1] = 255; //green
                pixelData[index + 2] = 255; //blue
                pixelData[index + 3] = 255; // alpha channel
            }
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate texture storage and upload the pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    

    //delete the pixel data after creating the texture
    delete[] pixelData;
    pixelData = nullptr;

    return textureID;
}

GLuint createDisplacementTexture(int width, int height) {
    // Implementation for creating a displacement texture
    // This can be similar to createColourTexture but with different pixel data generation logic
    // For example, you could create a texture that encodes height information for use in a shader
    return 0; // Placeholder return value
}