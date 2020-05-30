
#include "easySDL.h"

#include <iostream>

int rotX, rotY, rotZ = 0;
int mouseWheelY = 0;

void setup() {
    window3d("New game 1", 1280, 720);
//    window("New game 1", 1280, 720);
    vsyncMode(false);
    stroke(0, 0, 0, 0); // noStroke()
    fill(255);
}

void update() {
//    if (frameCount%(60) == 0) std::cout << "FPS: " << frameRate << " | FrameDelta: " << frameDelta <<  std::endl;
//    if (frameCount%(60) == 0) std::cout << "mouseX: " << mouseX << " | mouseY: " << mouseY <<  std::endl;
    background(200);

    pushMatrix();

    rotY = (float)mouseX/width*360;
    rotX = (float)mouseY/height*360;

//    fill((float)mouseX/width*255);
    stroke((float)mouseWheelY/60*255);

//    translate(mouseX, mouseY);
    translate(width/2, height/2);
    rotateX(radians(rotX));
    rotateY(radians(rotY));
    rotateZ(radians(rotZ));
    box(400, 200, 100);

    popMatrix();

    if (frameCount == 60*40) quit();
}

void mouseWheel(SDL_Event* event) { // TODO: Fix later
    mouseWheelY += event->wheel.y * ( (int)(event->wheel.direction == SDL_MOUSEWHEEL_NORMAL)*2 - 1 );
}


int main()
{
    printf("Hello world!\n");

    registerHandler(SDL_MOUSEWHEEL, mouseWheel);
    return easySDL::main(setup, update);

//    printf("Graceful exit.");
}