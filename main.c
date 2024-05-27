/*
 *--------------------------------------
 * Program Name: SNAKE
 * Author: Colin Stack
 * Description: it's a snake game :)'
 *--------------------------------------
*/

#include <string.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <keypadc.h>

#include <sys/timers.h>
#include <sys/lcd.h>

#include "gfx/gfx.h"

#define SNAKE_COLOR         247
#define BG_COLOR            0

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_FillScreen(2);
    
    int x = 0;
    int y = 0;
    gfx_sprite_t *behind_sprite = gfx_MallocSprite(16, 16);

    gfx_GetSprite(behind_sprite, x, y);
    gfx_TransparentSprite(coin, x, y);

    int xChange = 0;
    int yChange = 0;

    do {
        // keyboard input
        if(kb_Data[7] & kb_Up) {
            xChange = 0;
            yChange = -1;
        } else if (kb_Data[7] & kb_Down) {
            xChange = 0;
            yChange = 1;
        } else if (kb_Data[7] & kb_Left) {
            xChange = -1;
            yChange = 0;
        } else if(kb_Data[7] & kb_Right) {
            xChange = 1;
            yChange = 0;
        }
        
        //artificial delay
        uint24_t n = lcd_UpBaseCurr;
        msleep(1);
        while (n != lcd_UpBaseCurr) {
            n = lcd_UpBaseCurr;
            msleep(1);
        }

        //movement and sprite update
        gfx_Sprite(behind_sprite, x, y);

        x+=xChange; y+=yChange;

        x%=GFX_LCD_WIDTH;
        if(x < 0) x = GFX_LCD_WIDTH-1;
        y%=GFX_LCD_HEIGHT;
        if(y < 0) y = GFX_LCD_HEIGHT-1;

        gfx_GetSprite(behind_sprite, x, y);
        gfx_TransparentSprite(coin, x, y);

    } while (kb_Data[1] != kb_2nd);

    gfx_End();
    return 0;
}
