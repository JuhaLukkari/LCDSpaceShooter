/* Definitions and variables
*/
#define BULLETS 5
#define MONSTERS 10

//Game graphics made with special characters
byte bullet[8]{
  B00000,
  B00000,
  B00000,
  B01110,
  B00000,
  B00000,
  B00000,
};

byte ship[8]{
  B00000,
  B10000,
  B11100,
  B11111,
  B11100,
  B10000,
  B00000,
};

byte monster[8]{
  B00001,
  B11010,
  B00100,
  B00100,
  B00100,
  B11010,
  B00001,
};


//Structures for player, monsters etc.

typedef struct 
{
  int alive; //One hit to kill
  int score;
  int ypos; //Either 0 or 1, xpos is fixed 
}avatar;

typedef struct
{
  int alive; //One hit to kill
  int xpos;  //Between 0 and 15
  int ypos;  //One or zero
}enemy;

typedef struct
{
  int alive;  //If bullet has collided
  int xpos;
  int ypos;
}projectile;
