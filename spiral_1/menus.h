#ifndef MENUS_H_
#define MENUS_H_

#include "main.h"

int load(void);

int highscores(){
    int i;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    if (highscore == 0){
    	alt_up_char_buffer_string(char_buffer, "High Scores unavailable.", 30, 30);
    }else{
    	sprintf(temp_highscore, "High Score: %d", highscore);
    	alt_up_char_buffer_string(char_buffer, temp_highscore, 30, 30);
    }
    for (i=0; i<10000000; i++);
return 0;}

int win(){
    int i;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "YOU WIN!", 30, 30);
    for (i=0; i<10000000; i++);
return 0;}

int die(){
    int i;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    if (time_var1 == 90){
    	alt_up_char_buffer_string(char_buffer, "TIMES UP! YOU LOSE!", 30, 30);
    }else{
    	alt_up_char_buffer_string(char_buffer, "YOU LOSE!", 30, 30);}

    for (i=0; i<10000000; i++);
return 0;}

void menuhelper(sel, x){
    if (sel%3 == 0){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 100, x+10, 110, 0xf000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 150, x+10, 160, 0x0000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 200, x+10, 210, 0x0000, 0);
    }else if (sel%3 == 1){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 100, x+10, 110, 0x0000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 150, x+10, 160, 0xf000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 200, x+10, 210, 0x0000, 0);
    }else if (sel%3 == 2){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 100, x+10, 110, 0x0000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 150, x+10, 160, 0x0000, 0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, 200, x+10, 210, 0xf000, 0);
    }
}

int menu(){
    unsigned int sel = 999;
    int x = 80;

    alt_up_char_buffer_clear(char_buffer);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "---MENU---", 30, 10);
    alt_up_char_buffer_string(char_buffer, "Play", 30, 26);
    alt_up_char_buffer_string(char_buffer, "High Scores", 30, 39);
    alt_up_char_buffer_string(char_buffer, "Load game", 30, 51);

    menuhelper(sel, x);

    for(;;){
        dir mydir = getdir();
        if (mydir == down) menuhelper(++sel, x);
        else if (mydir == up) menuhelper(--sel, x);
        else if (mydir == left || mydir == right){
            if (sel%3 == 0){
                alt_up_char_buffer_clear(char_buffer);
                playgame();
            }else if (sel%3 == 1){
                alt_up_char_buffer_clear(char_buffer);
                highscores();
            }else if (sel%3 == 2){
                alt_up_char_buffer_clear(char_buffer);
                load();
            }
            break;
        }
    }
return 0;}


int load(){
    unsigned int sel = 999;
    int x = 80;

    alt_up_char_buffer_clear(char_buffer);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "---LOAD GAME---", 30, 10);
    alt_up_char_buffer_string(char_buffer, "Saved Game", 30, 26);
    alt_up_char_buffer_string(char_buffer, "From SD card", 30, 39);
    alt_up_char_buffer_string(char_buffer, "Cancel", 30, 51);

    menuhelper(sel, x);

    for(;;){
        dir mydir = getdir();
        if (mydir == down) menuhelper(++sel, x);
        else if (mydir == up) menuhelper(--sel, x);
        else if (mydir == left || mydir == right){
            if (sel%3 == 0){
                alt_up_char_buffer_clear(char_buffer);
                loadgame();
            }else if (sel%3 == 1){
                alt_up_char_buffer_clear(char_buffer);
                //sd_playgame();
                printf("IM HERE");
            }else if (sel%3 == 2){
                alt_up_char_buffer_clear(char_buffer);
                //Cancel, just return
            }
            break;
        }
    }
return 0;}

int pause(){
    int sel = 999;
    int x = 180;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(gridx/3), 0, (320/gridx)*(2*gridx/3), 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "---PAUSE---", 30, 10);
    alt_up_char_buffer_string(char_buffer, "Resume Game", 30, 26);
    alt_up_char_buffer_string(char_buffer, "Quit Game", 30, 39);
    alt_up_char_buffer_string(char_buffer, "Save & Quit", 30, 51);

    menuhelper(sel, x);
    for(;;){
        dir mydir = getdir();
        if (mydir == down) menuhelper(++sel, x);
        else if (mydir == up) menuhelper(--sel, x);
        else if (mydir == left || mydir == right){

            //Selected to Resume
            if (sel%3 == 0){
                alt_up_char_buffer_clear(char_buffer);
            return 0;}

            //Selected to Quit
            else if (sel%3 == 1){
                alt_up_char_buffer_clear(char_buffer);
            return 1;}

            //Selected to Save & quit
            else if (sel%3 == 2){
                alt_up_char_buffer_clear(char_buffer);
            return 2;}

            break;
        }
    }
    return 0;
}

int win_menu(){
    int sel = 999;
    int x = 180;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(gridx/3), 0, (320/gridx)*(2*gridx/3), 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "Continue To Next Level?", 26, 10);
    alt_up_char_buffer_string(char_buffer, "Yes", 30, 26);
    alt_up_char_buffer_string(char_buffer, "No", 30, 39);
    alt_up_char_buffer_string(char_buffer, "Replay Level", 30, 51);

    menuhelper(sel, x);
    for(;;){
        dir mydir = getdir();
        if (mydir == down) menuhelper(++sel, x);
        else if (mydir == up) menuhelper(--sel, x);
        else if (mydir == left || mydir == right){

            //Selected to Yes
            if (sel%3 == 0){
                alt_up_char_buffer_clear(char_buffer);
           level = 2;
           init_variables();
           playgame();
           }

            //Selected to No
            else if (sel%3 == 1){
                alt_up_char_buffer_clear(char_buffer);
            win();}

            //Selected to Replay
            else if (sel%3 == 2){
                alt_up_char_buffer_clear(char_buffer);
            init_variables();
            playgame();}

            break;
        }
    }
    return 0;
}

#endif /* MENUS_H_ */
