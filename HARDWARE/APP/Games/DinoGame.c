/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include "DinoGame.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* System Tick Configuration */
#define SYS_TICK_PER_SECOND 1000


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern unsigned long g_SystemTicks;
extern u8g2_t u8g2;


/*******************************************************************************
 * Code

 ******************************************************************************/
static int random(int low, int high)
{
  return rand() % (high - low + 1) + low;
}

static unsigned long millis(void)
{
    return ((unsigned long)g_SystemTicks  * 1000 / SYS_TICK_PER_SECOND );
}

// https://github.com/sarafong/T-Rex-Runner/blob/master/TRexRunner/TRexRunner.ino

//Multiple Dino Bitmaps

//--------------------Bitmaps--------------------//

//----------Dimensions----------//
#define smallcactus_width 8
#define smallcactus_height 17
#define standing_width 21
#define standing_height 20
#define frontLeg_width 21
#define frontLeg_height 20
#define backLeg_width 21
#define backLeg_height 20
#define gameover_width 99
#define gameover_height 35
#define easy_width 32
#define easy_height 15
#define medium_width 30
#define medium_height 20
#define hard_width 32
#define hard_height 15
#define insane_width 30
#define insane_height 20
#define menu_width 32
#define menu_height 15
#define replay_width 32
#define replay_height 15

//----------Bitmap Arrays----------//

static unsigned char smallcactus_bits[] = {
   0x18, 0x18, 0x98, 0x98, 0x99, 0x99, 0x99, 0x99, 0x79, 0x19, 0x1e, 0x18,
   0x18, 0x18, 0x18, 0x18, 0x18 };

static unsigned char standing_bits[] = {
   0x00, 0xf8, 0x0f, 0x00, 0xcc, 0x1f, 0x00, 0xf8, 0x1f, 0x00, 0xfc, 0x1f,
   0x00, 0xf8, 0x1f, 0x00, 0xfc, 0x00, 0x00, 0xf8, 0x07, 0x01, 0x3e, 0x00,
   0x01, 0x7f, 0x00, 0xc3, 0xff, 0x01, 0xef, 0x7f, 0x00, 0xff, 0x7f, 0x00,
   0xfe, 0x3f, 0x00, 0xfc, 0x3f, 0x00, 0xf8, 0x1f, 0x00, 0xf0, 0x07, 0x00,
   0xe0, 0x0e, 0x00, 0x60, 0x04, 0x00, 0x20, 0x08, 0x00, 0x60, 0x08, 0x00 };

static unsigned char frontLeg_bits[] = {
   0x00, 0xf0, 0x0f, 0x00, 0xdc, 0x1f, 0x00, 0xf8, 0x1f, 0x00, 0xf8, 0x1f,
   0x00, 0xf8, 0x1f, 0x00, 0xfc, 0x00, 0x00, 0xf8, 0x07, 0x01, 0x7e, 0x00,
   0x01, 0x7f, 0x00, 0xc3, 0xff, 0x01, 0xef, 0x7f, 0x01, 0xff, 0x7f, 0x00,
   0xfe, 0x7f, 0x00, 0xfc, 0x3f, 0x00, 0xf8, 0x1f, 0x00, 0xf0, 0x0f, 0x00,
   0xe0, 0x18, 0x00, 0xe0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x00, 0x00 };

static unsigned char backLeg_bits[] = {
   0x00, 0xf8, 0x0f, 0x00, 0xd8, 0x1f, 0x00, 0xfc, 0x1f, 0x00, 0xf8, 0x1f,
   0x00, 0xfc, 0x1f, 0x00, 0xf8, 0x00, 0x00, 0xfc, 0x07, 0x01, 0x3e, 0x00,
   0x01, 0x7f, 0x00, 0xc3, 0xff, 0x01, 0xef, 0x7f, 0x00, 0xff, 0x7f, 0x00,
   0xfe, 0x3f, 0x00, 0xfc, 0x3f, 0x00, 0xf8, 0x1f, 0x00, 0xf0, 0x07, 0x00,
   0x60, 0x0e, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x0c, 0x00 };

static unsigned char gameover_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x03, 0x0e, 0xd8, 0xc0, 0x07, 0x80, 0x07, 0x24,
   0xe0, 0x01, 0x0f, 0x00, 0x20, 0x00, 0x09, 0xf8, 0x40, 0x00, 0x80, 0x04,
   0x66, 0x30, 0x80, 0x09, 0x00, 0x20, 0x03, 0x19, 0xe8, 0xc0, 0x03, 0x80,
   0x04, 0x3c, 0xe0, 0x01, 0x0d, 0x00, 0x60, 0x02, 0x1b, 0x88, 0x40, 0x00,
   0xc0, 0x0c, 0x18, 0x20, 0x00, 0x07, 0x00, 0xc0, 0x03, 0x11, 0x98, 0xc0,
   0x07, 0x80, 0x07, 0x08, 0xe0, 0x01, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54,
   0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x7e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x1e, 0xe6, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x4e, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x6e, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xef, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0xef, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9e, 0xf4, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char easy_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x00, 0x00, 0x00, 0xc0, 0xf0, 0x00, 0x00, 0xf0, 0x10, 0x00, 0x00, 0xfc,
   0x50, 0x6d, 0x09, 0xff, 0x10, 0x2c, 0xc5, 0xff, 0x10, 0x4b, 0x06, 0xff,
   0xf0, 0x7e, 0x02, 0xfc, 0x00, 0x00, 0x02, 0xf0, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   
static unsigned char medium_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x22, 0x40, 0x01, 0x00, 0x66, 0x40, 0x00, 0x00,
   0xaa, 0x75, 0xd5, 0x0f, 0xda, 0x4f, 0x61, 0x08, 0x82, 0x48, 0x45, 0x09,
   0x42, 0x77, 0x5d, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
   0x00, 0xe0, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0xf0, 0x01, 0x00,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0xfc, 0x07, 0x00,
   0x00, 0xfc, 0x07, 0x00, 0x00, 0xfe, 0x0f, 0x00 };

static unsigned char hard_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
   0x07, 0x00, 0x00, 0x00, 0x1f, 0x40, 0x00, 0x20, 0x7f, 0x40, 0x04, 0x20,
   0xff, 0x41, 0xbb, 0x3b, 0xff, 0x43, 0x62, 0x24, 0xff, 0x41, 0x8c, 0x24,
   0x7f, 0x40, 0xb8, 0x38, 0x1f, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char insane_bits[] = {
   0x00, 0xfe, 0x0f, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0xfc, 0x07, 0x00,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0xf0, 0x01, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
   0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xc0, 0xdd, 0x9d, 0x03,
   0x50, 0x84, 0xa5, 0x03, 0x50, 0x52, 0x65, 0x00, 0x58, 0xdc, 0xa5, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char menu_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
   0x07, 0x00, 0x00, 0x00, 0x1f, 0x10, 0x01, 0x00, 0x7f, 0x30, 0x01, 0x00,
   0xff, 0xb1, 0xdd, 0x25, 0xff, 0x53, 0x5d, 0x26, 0xff, 0x11, 0x44, 0x4a,
   0x7f, 0x10, 0x5d, 0x3a, 0x1f, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char replay_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x00, 0x00, 0x00, 0xc0, 0x07, 0x20, 0x00, 0xf0, 0x09, 0x20, 0x00, 0xfc,
   0xe9, 0xae, 0x4b, 0xff, 0xe7, 0x32, 0xeb, 0xff, 0x15, 0xb2, 0x34, 0xff,
   0xe9, 0xae, 0x13, 0xfc, 0x00, 0x02, 0x10, 0xf0, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



//----------Initializing Variables----------//
static unsigned long dinoTimeToMove = 0; //Used to space out time(millis) between draws/moves
static unsigned long dinoTimeToDraw = 0;

static int score;
static int highScore = 0;
static int multiplier; //Score multiplier

//bool ignoreRepeat = false; //Avoid Switch Bouncing
static long compensation; //To compensate for the amount of time the Arduino has been running for after each run (essentially "resets" the time)
static bool gameOver;
static bool menu = true;

static bool down, jump; //Boolean values to check the state of the dinosaur
// static int dinoSwitch; //Switch between dino bitmaps to simulate movement
static int dinoY, cactiX1, cactiX2; //Positions of objects
static int velocity; //Speed of entire game
static int difficulty; //How fast the velocity increases


//--------------------Functions--------------------//

//Set/Reset Loop for Playing Again
void reset(){
  compensation = millis();
  gameOver = false;
  down = false;
  jump = false;
  // dinoSwitch = true;
  dinoY = 44;
  cactiX1 = 130;
  cactiX2 = cactiX1 + random(70, 120);
  velocity = 2;
}

//Deteching Joystick Interaction
void keyPress(void)
{
    uint8_t temp;
    
    temp = key_read();
    if(menu & (temp != 0))
    {
        reset();
        menu = false;
        if(temp == DOWN)
        {
            difficulty = 7500; 
            multiplier = 15000 / difficulty;
        }
        if(temp == UP)
        {
            difficulty = 2500; 
            multiplier = 15000 / difficulty;
        }
    }
    else if ( gameOver && (temp!=0) ) { //Left -> PlayAgain // Right -> Menu
        //ignoreRepeat = false;
        reset();
        if ( temp == UP ) menu = true; //Pushing Down
    }
    else if ( dinoY == 44 && temp == DOWN ) { //Corresponds to pushing up on the joystick
        jump = true;
        down = true;
    }
    Key_Open();
}

//Collision Detecting and Setting HighScore
void collision(void)
{
  if ( cactiX1 <= 23 && cactiX1 + smallcactus_width >= 15 && dinoY+20 >= 50 ) {
    if ( (millis()-compensation)*multiplier/250 > highScore && !gameOver ) highScore = (millis()-compensation)*multiplier/250; //Changes highscore if current score is greater
    gameOver = true;
  }
}

void moveDino(void)
{
  if ( dinoY > 44 ) dinoY = 44; //Resets dinosaur to it passes it
  if ( jump || dinoY <= 43 ) { //Allows jumping if on the ground
    jump = false;
    if ( dinoY >= 16 && down ) dinoY -= velocity; //Going up
    else { //Going down
      down = false;
      dinoY += velocity;
    }
  }
}

void moveCactus(void)
{
  cactiX1 -= velocity;
  cactiX2 -= velocity;
  if ( cactiX1 <= -15 ) { //Once cacti reaches left side of screen, resets to right side of screen
    cactiX1 = cactiX2;
    cactiX2 = cactiX1 + random(70, 150);
  }
}

//-------Move Functions-------//
void moveObjects(void)
{
    if( millis() > dinoTimeToMove+50 )
    { //Updates every 50 milliseconds
        velocity = (millis()-compensation)/difficulty + 2; //Increases velocity as game progresses
        moveDino();
        moveCactus();
        dinoTimeToMove = millis();  
    }
}


//-------Draw Functions-------//
void drawDinoCactus(void)
{
    if ( dinoY < 44 ) u8g2_DrawXBM(&u8g2, 10, dinoY, standing_width, standing_height, standing_bits); //While jumping don't change bitmap
    else if ( (millis()-compensation) % 16 > 9 ) u8g2_DrawXBM(&u8g2, 10, dinoY, frontLeg_width, frontLeg_height, frontLeg_bits); //Switch between bitmaps every 9 millis
    else u8g2_DrawXBM(&u8g2, 10, dinoY, backLeg_width, backLeg_height, backLeg_bits);
    u8g2_DrawXBM(&u8g2, cactiX1, 47, smallcactus_width, smallcactus_height, smallcactus_bits); //Draw cacti
    u8g2_DrawXBM(&u8g2, cactiX2, 47, smallcactus_width, smallcactus_height, smallcactus_bits);
}

void drawScore(void)
{
    char scoreBuff[50];
    if ( menu ) { //Print highscore on menu screen
        score = highScore;
        snprintf(scoreBuff, 50, "HighS: %d", score);
    }
    else if ( !gameOver )
    { //Increments score ONLY while playing
        score = (millis() - compensation) * multiplier / 250;
        snprintf(scoreBuff, 50, "Score: %d", score);
    }
    u8g2_SetFont(&u8g2, u8g2_font_t0_11_mf);
    u8g2_DrawStr(&u8g2, 55, 8, scoreBuff );
}

//Draws Game Over Screen
void playAgain(void)
{ 
  u8g2_DrawXBM(&u8g2, 14, 14, gameover_width, gameover_height, gameover_bits);
  u8g2_DrawXBM(&u8g2, 23, 30, replay_width, replay_height, replay_bits);
  u8g2_DrawXBM(&u8g2, 70, 30, menu_width, menu_height, menu_bits);
}

//Draws Menu Screen
void menuScreen(void)
{
  u8g2_DrawXBM(&u8g2, 26, 30, easy_width, easy_height, easy_bits);
  u8g2_DrawXBM(&u8g2, 49, 12, medium_width, medium_height, medium_bits);
  u8g2_DrawXBM(&u8g2, 69, 30, hard_width, hard_height, hard_bits);
  u8g2_DrawXBM(&u8g2, 49, 43, insane_width, insane_height, insane_bits);
}

void draw_DinoGame(void)
{
    if( millis() > dinoTimeToDraw + 25 ){ //Draws every 25 milliseconds
    do {
        drawDinoCactus();
        drawScore();
        if ( menu ) menuScreen(); //Draw Menu Screen
        else if ( gameOver ) playAgain(); //Draw Game Over Screen
    } while( u8g2_NextPage(&u8g2) );
    dinoTimeToDraw = millis();
  }
}

/*!
 * @brief DinoGame_Task function
 */
void DinoGame_Run(void)
{
    /* set systick and start systick interrupt */
    SysTick_Config(SystemCoreClock/SYS_TICK_PER_SECOND);    

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    //u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
    u8g2_SetFont(&u8g2, u8g2_font_t0_11_mf);
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFontRefHeightAll(&u8g2);
    reset();
    while (1)
    {
      if (key_read()==ENTER)
      {
        Key_Open();
        break;
      }
      u8g2_FirstPage(&u8g2);
      keyPress();
      if ( (!gameOver) && (!menu) )
      {
        moveObjects();
      }
      draw_DinoGame();
      collision();
    }   
}
