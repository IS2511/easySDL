
#include "easySDL.h"


void setup() {
    // Here goes code
//    window3d("New game 1", 1280, 720);
    window("New game 1", 1280, 720);
//    vsyncMode(true);
    delay(5000);
    quit();
}

void update() {
    // Here goes code
}


int main()
{
    printf("Hello world!\n");
    printf("Pointers: %p :  %p\n", setup, main);
    easySDL::main(setup, update);
    printf("Graceful exit.");
}