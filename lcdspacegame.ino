/*

 Space shooter for 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 *Two switches (or buttons) with respective outputs going to digital pins 7 and 6 
 *ends to ground and to +5V through a resistor (some hunderds of ohms possibly enough)
*/

// include the library code:
#include <LiquidCrystal.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("SPACE INVADERS"); //Is not printed if is printed after createChar, mysterious
  pinMode(7,INPUT);          //Set pin 7 as input for moving the ship
  pinMode(6,INPUT);          //Set pin 6 as input for firing
  lcd.createChar(0, bullet); //Create own characters
  lcd.createChar(1, ship);
  lcd.createChar(2, monster);
  //Characters that don't fit to the LCD will not be printed
  delay(3000);
  lcd.clear();  //Clear the screen
  Serial.begin(9600);
}


void game_end(int score)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You died!");
  lcd.setCursor(0,1);
  lcd.print("SCORE:");
  lcd.print(score);
  delay(5000);
  loop();
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.clear();
  lcd.setCursor(0, 0);
  //Create structs for game objects
  avatar player = {1,0,0}; //Alive, score, ypos
  enemy enemies[15];  //Start enemies list from the last position
  enemies[0]={1,15,0};
  projectile projectiles[31]; //Same for projectiles
  //Draw our ship (Could be X-wing and enemies tie fighters!)
  //Writing only if monster is alive
  //For loop for monsters that upgrades their position etc
  //Counter that creates new monster periodically
  int button1=0;    //Moving ship
  int button2=0;    //Firing
  int oldpos=0;     //For clearing screen
  int k=0;          //For counting 
  int monstn=0;     //Number of monsters on screen
  int projn=0;      //Number of projectiles on screen
  int rproj=0;      //For resetting projectiles
  int rmonst=0;     //For resetting monsters
  int state=1;      //State of the game, 1 = game loop, 0 = game_end()
  while(state==1)
  {
    //MOVEMENT
    button1=digitalRead(7);
    if(button1==0)
    {
      if(player.ypos==1)
        player.ypos=0;
      else if(player.ypos==0)
        player.ypos=1;
      
    }
    lcd.setCursor(0,player.ypos);
    lcd.write(byte(1)); //Player
    //Clear player when moved
    if(oldpos!=player.ypos)
    {
      lcd.setCursor(0,oldpos);
      lcd.print(" ");
      oldpos=player.ypos;
    }

    //SHOOTING
    button2=digitalRead(6);
    if(button2==0 && projn<BULLETS)
    {
      projectiles[projn]={1,1,player.ypos}; //Create new projectile
      projn++;  //Increment amount of projectiles on screen
      
    }
    //Reset projectiles
    //Perhaps make some overheat thing
    //Wait for some seconds before shooting
    if(rproj>=BULLETS)
    {
      projn=0;
      rproj=0;
    }
    //Reset monsters
    if(rmonst>=MONSTERS)
    {
      monstn=0;
      rmonst=0;
    }
    int i=0;
    int j=rand() % 2; 
    int a=1;
    //Draw monsters
    for(i;i<monstn;i++)
    {
      if(enemies[i].alive!=0)
      {
        lcd.setCursor(enemies[i].xpos,enemies[i].ypos);
        lcd.write(byte(2));
        //lcd.print(enemies[i].alive); //This for debugging
        //Check for collision with player
        if(enemies[i].xpos==0 && enemies[i].ypos==player.ypos)
        {
          game_end(player.score);
          state=0; //Exit while loop
        }
        //Check if monsters have passed the screen
        if(enemies[i].xpos<0)
        {
          enemies[i].alive=0;
          //monstn--;
          rmonst++;
        }
        //Check for collision with projectiles
        int p=0;
        for(p;p<projn;p++)
        {
          if((enemies[i].xpos==projectiles[p].xpos || enemies[i].xpos==projectiles[p].xpos-1) && enemies[i].ypos==projectiles[p].ypos && projectiles[p].alive==1)
          {
            enemies[i].alive=0;     //Kill enemy
            //Clear enemy (and projectile)
            lcd.setCursor(enemies[i].xpos,enemies[i].ypos);
            lcd.print(" ");
            //monstn--;
            rmonst++;
            player.score++;         //Increment player score
            projectiles[p].alive=0; //Projectile exploded
            //projn--;
            rproj++;
          }
        }
      }
    }
    //Draw projectiles
    int c=0;
    for(c;c<projn;c++)
    {
      if(projectiles[c].alive!=0)
      {
        lcd.setCursor(projectiles[c].xpos,projectiles[c].ypos);
        lcd.write(byte(0));
        //lcd.print(projectiles[c].alive);  //For debugging
        //Remove projectiles that have crossed the screen
        if(projectiles[c].xpos>16)
        {
          projectiles[c].alive=0;
          //projn--;
          rproj++;
        }
      }
    }
    
     
    delay(400);
    //Clear screen
    for(i=0;i<monstn;i++)
    {
      if(enemies[i].alive!=0)
      {
        lcd.setCursor(enemies[i].xpos,enemies[i].ypos);
        lcd.print(" ");
        enemies[i].xpos--; //Advance enemies
      }
    }
    for(c=0;c<projn;c++)
    {
      if(projectiles[c].alive!=0)
      {
        lcd.setCursor(projectiles[c].xpos,projectiles[c].ypos);
        lcd.print(" ");
        projectiles[c].xpos++;
      }
    }


    k++;
    //Create new monsters periodically
    //With higher level the period decreases
    //Amount of delay could also decrease
    if(k==5 && rmonst<MONSTERS)
    {
      monstn++;                   //Increase number of monsters (on the screen)
      int y=rand() % 2;           //First or second row
      enemies[monstn-1]={1,15,y}; //Initialize monster
      k=0;
    }
  }
}

