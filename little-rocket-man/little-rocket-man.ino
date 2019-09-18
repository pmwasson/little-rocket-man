/*
Hello, World! example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.
This code is the most basic barebones code for writing a program for Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include <Arduboy2.h>

// make an instance of arduboy used for many functions
Arduboy2 arduboy;

uint16_t background;
int8_t  movx;
int8_t  movy;

#define PRIME1 613
#define PRIME2 3001
#define MAXX   6
#define MAXY   5

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(60);

  background = 0;
  movx = 0;
  movy = 0;
}


// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();

  // we set our cursor 5 pixels to the right and 10 down from the top
  // (positions start at 0, 0)
  arduboy.setCursor(10, 11);

  // then we print to screen what is in the Quotation marks ""
  arduboy.print(background);

  for(uint16_t i=0; i < 128*64; i+=PRIME1) {
    uint16_t j = i + background;
    arduboy.drawPixel (j & 0x7f, (j >> 7) & 0x3f, WHITE);
  }

  for(uint16_t i=0; i < 128*64*4; i+=PRIME2) {
    uint16_t j = i + background;
    arduboy.drawPixel ((j>>1) & 0x7f, (j >> 8) & 0x3f, WHITE);
  }

  if (arduboy.pressed(RIGHT_BUTTON)) {
    movx = min(movx+1,MAXX);
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    movx = max(movx-1,-MAXX);
  }
  if (arduboy.pressed(UP_BUTTON)) {
    movy = min(movy+1,MAXY);
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    movy = max(movy-1,-MAXY);
  }
  if (arduboy.pressed(A_BUTTON)) {
    movx = 0;
    movy = 0;
  }
  
  background += movx + (movy << 7);

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
