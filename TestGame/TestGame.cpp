
#include "easySDL.h"

#include <iostream>

void setup() {
    // Here goes code
    window3d("New game 1", 1280, 720);
//    window("New game 1", 1280, 720);
    vsyncMode(false);
    stroke(0, 0, 0, 0); // noStroke()
    fill(255);
}

void update() {
    // Here goes code
//    if (frameCount%(60) == 0) std::cout << "FPS: " << frameRate << " | FrameDelta: " << frameDelta <<  std::endl;
    if (frameCount%(60) == 0) std::cout << "mouseX: " << mouseX << " | mouseY: " << mouseY <<  std::endl;
    background(200);

    pushMatrix();

    translate(mouseX, mouseY);
    rotateY(radians(10));
    rotateX(radians(10));
    box(720);

    popMatrix();

    if (frameCount == 60*40) quit();
}


int main()
{
    printf("Hello world!\n");
    printf("Pointers: %p :  %p\n", setup, main);

    easySDL::main(setup, update);

    printf("Graceful exit.");
}