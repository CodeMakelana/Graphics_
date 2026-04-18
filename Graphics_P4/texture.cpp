#include "texture.h"
#include <cmath>
#include <stdio.h>

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
            
            float dist = sqrt(((localX - centerX) * (localX - centerX)) + ((localY - centerY) * (localY - centerY)));

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
            
            float dist = sqrt(((localX - centerX) * (localX - centerX)) + ((localY - centerY) * (localY - centerY)));

            int index = (y * width + x) * 4; // index for the pixel array


            if (dist < radius) { // a dimple.
                //write a darker colour to the pixel data 
                pixelData[index] = 50;       //red
                pixelData[index + 1] = 50;   //green
                pixelData[index + 2] = 50;   //blue
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    

    //delete the pixel data after creating the texture
    delete[] pixelData;
    pixelData = nullptr;

    return textureID;
}

//createAlphaTexture(int width, int height)
//Same dimple logic again. Outside dimples write 255 (fully opaque), inside dimples write something low like 30 (mostly transparent). The exact transparency is controlled in the shader, where it is multiplied by the uniform alpha value. Upload as a single-channel GL_RED texture or GL_RGBA for simplicity.

GLuint createAlphaTexture(int width, int height) {
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
            
            float dist = sqrt(((localX - centerX) * (localX - centerX)) + ((localY - centerY) * (localY - centerY)));

            int index = (y * width + x) * 4; // index for the pixel array

            if (dist < radius) { // a dimple.
                //write a darker colour to the pixel data 
                pixelData[index] = 30;       //red
                pixelData[index + 1] = 30;   //green
                pixelData[index + 2] = 30;   //blue
                pixelData[index + 3] = 30;    // alpha channel (mostly transparent)
            } else { // a flat area
                //write a lighter colour to the pixel data 
                pixelData[index] = 255;     //red
                pixelData[index + 1] = 255; //green
                pixelData[index + 2] = 255; //blue
                pixelData[index + 3] = 255; // alpha channel (fully opaque)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    //delete the pixel data after creating the texture
    delete[] pixelData;
    pixelData = nullptr;

    return textureID;
}

void bindTexture(GLuint textureID, int unit, GLuint shaderProgram, const char* uniformName) {
    glActiveTexture(GL_TEXTURE0 + unit); // Activate the specified texture unit
    glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture to the active unit

    // Set the uniform in the shader to point to the correct texture unit
    GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    if (uniformLocation != -1) {
        glUniform1i(uniformLocation, unit); // Tell the shader which texture unit to use
    } else {
        // Handle error: uniform not found
        fprintf(stderr, "Uniform '%s' not found in shader program %d\n", uniformName, (int)shaderProgram);

    }
}