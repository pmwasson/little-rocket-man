/*
Little Rocket Man
Sept. 2019
Paul Wasson
*/

#include <Arduboy2.h>
#include <Sprites.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
Sprites  sprites;
ArduboyTones sound(arduboy.audio.enabled);

#include "map.h"
#include "sprite.h"


#define SWIDTH     128
#define SHEIGHT    64

#define STARFIELD1 613
#define STARFIELD2 3001
#define MAXMOVX   5
#define MAXMOVY   5

#define COLLISION_NONE 0
#define COLLISION_FLOOR 1
#define COLLISION_CEILING 2
#define COLLISION_RIGHT 4
#define COLLISION_LEFT 8


uint16_t background;
int8_t   movx;
int8_t   movy;
int16_t  posx;
int16_t  posy;
uint8_t  collision;

// Setup
void setup() {
  // initiate arduboy instance
  arduboy.begin();
  arduboy.setFrameRate(60);

  background = 0;
  movx = 0;
  movy = 0;
  posx = MAPSTARTX*128;
  posy = MAPSTARTY*128;
}


// Game loop
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;



  // Drawing walls/spaces will write every byte of the
  // screen buffer, so no need to clear

  // 128x128 block , 16x16 map
  // if delx > 0, need 2 blocks in x
  // if dely > 64, need 2 blocks in y
  uint8_t mapx = (posx>>7) & 0xf;
  uint8_t mapy = (posy>>7) & 0xf;
  uint16_t map_ptr = mapx + mapy*16;
  uint8_t delx = posx & 0x7f;
  uint8_t dely = posy & 0x7f;
  
  collision = COLLISION_NONE;
  
  uint8_t tile = pgm_read_byte(lrm_map+map_ptr);
  drawBottomWall(0,128-delx,128-dely,tile);
  checkFloor(tile,dely,delx<72);
  checkLeft(tile,delx,dely<108);
  
  if (delx > 0) {
    tile = pgm_read_byte(lrm_map+map_ptr+1);
    drawBottomWall(128-delx,128,128-dely,tile);
    checkFloor(tile,dely,delx>56);
    checkRight(tile,delx,dely<108);
  }
  
  if (dely > 64) {
    tile = pgm_read_byte(lrm_map+map_ptr+16);
    drawTopWall(0,128-delx,dely-64,tile);
    checkCeiling(tile,dely,delx<72);
    checkLeft(tile,delx,dely>84);
    
    if (delx > 0) {
      tile = pgm_read_byte(lrm_map+map_ptr+17);
      drawTopWall(128-delx,128,dely-64,tile);
      checkCeiling(tile,dely,delx>56);
      checkRight(tile,delx,dely>84);
    }
  }

  if (collision & COLLISION_FLOOR) {
    posy = (posy & 0xff80) + 96 + 12;
    movy = 0;
  }
  if (collision & COLLISION_CEILING) {
    posy = (posy & 0xff80) + 96 - 12;
    movy = 0;
  }
  if (collision & COLLISION_RIGHT) {
    posx = (posx & 0xff80) + 64 - 8;
    movx = 0;
  }
  if (collision & COLLISION_LEFT) {
    posx = (posx & 0xff80) + 64 + 8;
    movx = 0;
  }
  
  // Debug
  arduboy.setCursor(0, 28);
  arduboy.print(posx);
  arduboy.setCursor(56, 0);
  arduboy.print(posy);
  arduboy.setCursor(8*14, 28);
  arduboy.print(collision);

  drawStars(background,STARFIELD1,0);
  drawStars(background,STARFIELD2,1);

  // Inputs
  uint8_t frame = 0;
  
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (!(collision & COLLISION_RIGHT)) {
      movx = min(movx+1,MAXMOVX);
      if (collision & COLLISION_FLOOR) {
        frame = (arduboy.frameCount>>2&1);
        if (!sound.playing()) {
          sound.tone(NOTE_C3,100, NOTE_REST,80);
        }
      }
    }
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    if (!(collision & COLLISION_LEFT)) {
      movx = max(movx-1,-MAXMOVX);
      if (collision & COLLISION_FLOOR) {
        frame = (arduboy.frameCount>>2&1);
        if (!sound.playing()) {
          sound.tone(NOTE_C3,100, NOTE_REST,80);
        }
      }
    }
  }
  else if (collision & COLLISION_FLOOR) {
    movx=0;
  }

  if (arduboy.pressed(B_BUTTON)) {
    if (!sound.playing()) {
      sound.tone(NOTE_C2H,60, NOTE_A2H,30); 
    }
    frame = 2+(arduboy.frameCount>>2&1);
    if (!(collision & COLLISION_CEILING)) {
      movy = min(movy+1,MAXMOVY);
    }
  }
  else if (!(collision & COLLISION_FLOOR)) {
    movy = max(movy-1,-MAXMOVY);
    frame=1;
  }
  else if (arduboy.pressed(UP_BUTTON)) {
    frame=1;
  }

  sprites.drawPlusMask((128/2)-(16/2),(64/2)-(24/2),lrm_plus_mask,frame);
  
  posy += movy;
  posx += movx;
  background += -movx + (movy << 7);

  if (arduboy.pressed(A_BUTTON)) {
    background = 0;
    movx = 0;
    movy = 0;
    posx = MAPSTARTX*128;
    posy = MAPSTARTY*128;    
  }

  
  arduboy.display();
}

void checkFloor(uint8_t tile, uint8_t dely, bool x_good) {
  if ((tile != 0) && (dely <= 108) && (dely > 108-7) && x_good) {
    collision |= COLLISION_FLOOR;
  }
}

void checkCeiling(uint8_t tile, uint8_t dely, bool x_good) {
  if ((tile !=0) && (dely >= 84) && (dely < 84+7) && x_good) {
    collision |= COLLISION_CEILING;
  }
}

void checkLeft(uint8_t tile, uint8_t delx, bool y_good) {
    if ((tile != 0) && (delx <= 72) && (delx > 72-7) && y_good) {
      collision |= COLLISION_LEFT;
    }
}

void checkRight(uint8_t tile, uint8_t delx, bool y_good) {
    if ((tile != 0) && (delx >= 56) && (delx < 56+7) && y_good) {
      collision |= COLLISION_RIGHT;
    }
}

void drawStars(uint16_t background, uint16_t seed, uint8_t layer) {
  for(uint16_t i=0; i < SWIDTH*SHEIGHT; i+=seed) {
    uint16_t j = i + background;
    arduboy.drawPixel ((j>>layer) & 0x7f, (j >> (7+layer)) & 0x3f, WHITE);
  }
}

// Assume we call bottom first, so top need to OR on the overlap
// y is the number of lines to draw from the top
void drawTopWall(uint8_t x0, uint8_t x1, uint8_t y, uint8_t tile) {
  uint16_t line = 0;
  uint8_t bits = tile ? 0xff : 0;
  while (y > 0) {
    if (y < 8) {
      bits = bits>>(8-y);
      y=0;
    }
    else {
      y -= 8;
    }
    for (uint8_t x=x0; x < x1; x++) {
      if (y > 0) {
        arduboy.sBuffer[line+x] = bits;
      }
      else {
        arduboy.sBuffer[line+x] |= bits;        
      }
    }
    line += 128;
  }
}

// Note, y is flipped from normal (number of lines to draw from the bottom)
void drawBottomWall(uint8_t x0, uint8_t x1, uint8_t y, uint8_t tile) {
  uint16_t line = 128*7;  // Bottom row
  uint8_t bits = tile ? 0xff : 0;
  y = min(64,y); // check y bounds
  while (y > 0) {
    if (y < 8) {
      bits = bits<<(8-y);
      y=0;
    }
    else {
      y -= 8;
    }
    for (uint8_t x=x0; x < x1; x++) {
      arduboy.sBuffer[line+x] = bits;
    }
    line -= 128;
  }
}
