#ifndef DRAW_LEVEL_H_
#define DRAW_LEVEL_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>

#include "play_game.h"


//------------------------------------------------------
// Level design
typedef enum { NoBlock, Grass, Water, Highway, WinBlock, Highway_t } background;
#define c_NoBlock   0x0000
#define c_Grass     0x0f00
#define c_Water     0x00f0
#define c_Highway   8 | (8 << 6) | (8 << 11)
#define c_WinBlock  0x0f10
#define c_Yellow	0xff00


//------------------------------------------------------

char time_remaining[20];
char time_r[5];
background g[50][50];


void draw_topinfo(){
    if((time_var1 - 90) >= 10){
		strncpy(time_remaining, "Time Remaining: ", sizeof(time_remaining));
		sprintf(time_r, "%d", time_var1-90);
		strncat(time_remaining, time_r , 2);
		alt_up_char_buffer_string(char_buffer, time_remaining, 0, 0);
    }else{
    	strncpy(time_remaining, "Time Remaining: ", sizeof(time_remaining));
		sprintf(time_r, " %d", time_var1-90);
		strncat(time_remaining, time_r , 2);
		alt_up_char_buffer_string(char_buffer, time_remaining, 0, 0);
    }
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 90, 0, 150, 3, 0x0F00, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 150, 0, time_var1, 3, 0xFFFF, 1);
//Score
    if((time_var1-90)*(lives_remaining) >= 100){
		sprintf(score, "Score: %d", (time_var1-90)*(lives_remaining));
		alt_up_char_buffer_string(char_buffer, score, 0, 4);
    }else if((time_var1-90)*(lives_remaining) >= 10){
        sprintf(score, "Score:  %d", (time_var1-90)*(lives_remaining));
        alt_up_char_buffer_string(char_buffer, score, 0, 4);
	}else if((time_var1-90)*(lives_remaining) >= 0){
		 sprintf(score, "Score:   %d", (time_var1-90)*(lives_remaining));
		 alt_up_char_buffer_string(char_buffer, score, 0, 4);
	}

    //Lives Remaining
    switch (lives_remaining){
        case 3:
            alt_up_char_buffer_string(char_buffer, "Lives Remaining: 3", 0, 2);
            break;
        case 2:
            alt_up_char_buffer_string(char_buffer, "Lives Remaining: 2", 0, 2);
            break;
        case 1:
            alt_up_char_buffer_string(char_buffer, "Lives Remaining: 1", 0, 2);
            break;
        default:
            alt_up_char_buffer_string(char_buffer, "Lives Remaining:?", 0, 2);
            break;
    }
}


void printgrid(){
    int i,j = 0;

    for (i=0; i<gridy; i++){
        for (j=0; j<gridx; j++){
            if      (g[i][j] == NoBlock)  /////Don't really need to draw this - Amitoj////////
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_NoBlock, 1);
             if (g[i][j] == Grass)
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Grass, 1);
            else if (g[i][j] == Water){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Water, 1);
            	movement_matrix[i][j] = 1;
            }
            else if (g[i][j] == Highway)
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Highway, 1);
            else if (g[i][j] == WinBlock)
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_WinBlock, 1);
            else if (g[i][j] == Highway_t){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Highway, 1);
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i)+1, c_Yellow, 1);
            }

        }
    }
}

void setup_level(){
    int i;
    for ( i=0; i<gridx; i++ ){
        g[0][i] = NoBlock;
        g[1][i] = WinBlock;

        g[2][i] = Water;
        g[3][i] = Water;
        g[4][i] = Water;

        g[5][i] = Grass;
        g[5][2] = WinBlock;
        g[6][i] = Highway;

        	if(i%2){
        		g[7][i] = Highway;
        		g[8][i] = Highway;
        	}
        	else{
        		g[7][i] = Highway_t;
        		g[8][i] = Highway_t;
        	}

        g[9][i] = Grass;
        g[10][i] = Grass;
        g[11][i] = Grass;
        g[12][i] = Grass;
    }
}


#endif
