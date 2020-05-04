
#include "easySDL.h"


void setup() {
    // Here goes code
//    window3d("New game 1", 1280, 720);
    window("New game 1", 1280, 720);
//    vsyncMode(true);

}

void update() {
    // Here goes code
}


int main()
{
    easySDL::main(setup, update);
}