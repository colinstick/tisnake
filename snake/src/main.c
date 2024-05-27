/*
 *--------------------------------------
 * Program Name: SNAKE
 * Author: Colin Stack
 * Description: it's a snake game :)
 *--------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <keypadc.h>

#include <sys/timers.h>
#include <sys/lcd.h>

#include "gfx/gfx.h"
#define BG_COLOR            0

int arr[15][20]; // 13x18

int headR = 0;
int headC = 0;

int tailR = 0;
int tailC = 0;

int currDirection = 1; //0 for not moving, 1 for right, 2 for up, 3 for left, 4 for down

int appleR = 5;
int appleC = 7;

bool dead = false;


void checkDeath() {
    // does the player deserve to die?
    if(headR > 14 || headR < 0) {
        dead = true;
        return;
    }
    if(headC > 19 || headC < 0) {
        dead = true;
        return;
    }
    if(arr[headR][headC] != 0) {
        dead = true;
        return;
    }
}

void updateHead() {
    //update arr start to point to next block
    arr[headR][headC] = currDirection;
    //update start variable
    headR += (currDirection==2?-1:0);
    headR += (currDirection==4?1:0);

    headC += (currDirection==1?1:0);
    headC += (currDirection==3?-1:0);

    //ensure new head does not cause death
    checkDeath();

    //draw sprite at new area
    gfx_Sprite(block, headC*16, headR*16);
}

void updateTail() {;
    //remove sprite there
    gfx_Sprite(blackblock, tailC*16, tailR*16);
    //update tail variable
    if(arr[tailR][tailC] == 1) {
        arr[tailR][tailC] = 0;
        tailC++;
    } else if(arr[tailR][tailC] == 2) {
        arr[tailR][tailC] = 0;
        tailR--;
    } else if(arr[tailR][tailC] == 3) {
        arr[tailR][tailC] = 0;
        tailC--;
    } else if(arr[tailR][tailC] == 4) {
        arr[tailR][tailC] = 0;
        tailR++;
    }
}

void placeApple() {
    //gen coords for apple
    int tC=rand() % 18; 
    int tR=rand() % 13;
    while(arr[tR][tC] != 0) {
       tC = rand() % 18;
       tR = rand() % 13;
    }

    appleR = tR;
    appleC = tC;

    //place apple
    gfx_TransparentSprite(apple, appleC*16, appleR*16);
}

int main(void)
{
    srand(time(NULL));
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_FillScreen(2);
 
    gfx_Sprite(block, 0, 0);
    
    bool growing=false;
    
    timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);

    for(int i=0; i<13; i++) {
        for(int j=0; j<18; j++)
            arr[i][j] = 0;
    }
    arr[0][0] = 1;

    gfx_TransparentSprite(apple, appleC*16, appleR*16);
    
    do {
        // keyboard input
        if(kb_Data[7] & kb_Up) {
            currDirection = 2;
        } else if (kb_Data[7] & kb_Down) {
            currDirection = 4;
        } else if (kb_Data[7] & kb_Left) {
            currDirection = 3;
        } else if(kb_Data[7] & kb_Right) {
            currDirection = 1;
        }

        //only update game every fifth of a second
        if(timer_Get(1) > 6400) {
            timer_Set(1, 0); 
        } else {
            continue;
        }
        
        //remove block at back of snake
        if(!growing)
            updateTail();
        else
            growing = false;

        //add block at head of snake 
        updateHead();

        //on an apple?
        if(headR == appleR && headC == appleC) {
            placeApple();
            growing = true;
        }
        

    } while (kb_Data[1] != kb_2nd && dead==false);

    gfx_End();
    return 0;
}
