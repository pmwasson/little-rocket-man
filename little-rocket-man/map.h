// Map (nibbles) - 32
// block = 128x128 (10m x 10m)
// map = 64x64 (blocks) = 4k bytes
// Note, flipping Y coordinate to match game goals

// 0 = ...
//     ...
//     ...
//
// 1 = ###
//     ###
//     ###
//
// 2 = ###.
//     ##.
//     #..
//
// 3 = ###
//     .##
//     ..#
//
// 4 = #..
//     ##.
//     ###
//
// 5 = ..#
//     .##
//     ###

#define MAP_VOID  0
#define MAP_WALL  1
#define MAP_UL    2
#define MAP_UR    3
#define MAP_LL    4
#define MAP_LR    5

const uint8_t PROGMEM lrm_map[] =
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 0
  1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,1, // 1
  1,1,0,0,1,1,1,0,1,1,1,0,1,0,1,1, // 2
  1,1,1,0,1,1,1,0,1,0,0,0,1,0,1,1, // 3
  1,0,1,0,0,0,0,0,1,1,1,1,1,0,1,1, // 4
  1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1, // 5
  1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1, // 6
  1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1, // 7
  1,1,0,1,1,0,1,0,1,1,1,1,1,0,1,1, // 8 
  1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1, // 9
  1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1, // 10
  1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1, // 11
  1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,1, // 12
  1,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1, // 13
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, // 14
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1  // 15
//0 1 2 3 4 5 6 7 8 9 1 1 1 1 1 1
//                    0 1 2 3 4 5
};

#define MAPSTARTX 2
#define MAPSTARTY 2
