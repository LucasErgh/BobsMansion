/*
    main.cpp
*/

#include "Game.hpp"

int main(void){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Bob's Mansion");
    InitAudioDevice();
    DisableCursor();
    SetTargetFPS(60);

    Game game;
    game.InitializeWindow();
    game.run();

}


