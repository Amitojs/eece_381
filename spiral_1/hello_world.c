#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>

#include "initilize_vga.h"
#include "draw_vehicles.h"
#include "play_game.h"
#include "main.h"
#include "draw_level.h"
#include "menus.h"
#include "save.h"



//------------------------------------------------------
// Global Variables
    dir lastdir;
//------------------------------------------------------

void init_matrix(){
    int i, j = 0;
    for (i=0; i<gridy; i++){
        for (j=0; j<gridx; j++){
            movement_matrix[i][j] = 0;
        }
    }
}

void init_variables(){
	frog_x = (320/gridx)*6;
	frog_y = (240/gridy)*9;
	lives_remaining = 3;
	time_var1 = 150;
	time_var2 =0;
	truck_1 = (320/gridx)*0;
	truck_2 = (320/gridx)*8;

	cars[0] = (320/gridx)*1;
	cars[1] = (320/gridx)*6;
	cars[2] = (320/gridx)*11;
	cars[3] = (320/gridx)*2.5;
	cars[4] = (320/gridx)*7.5;
	cars[5] = (320/gridx)*12.5;

	logs[0]	= (320/gridx)*1;
	logs[1]	= (320/gridx)*5;
	logs[2]	= (320/gridx)*2.5;
	logs[3]	= (320/gridx)*7.5;
	logs[4]	= (320/gridx)*3.5;
	logs[5]	= (320/gridx)*10;
}

int checkwin(){
   if (g[frog_y/(240/gridy)][frog_x/(320/gridx)] == WinBlock){
	   return 1;
   }
   else
       return 0;
}

int swfinder(){
    return IORD_8DIRECT(YOUR_SWITCHES_ADDR, 0);
}

dir getdir(){
    if       (swfinder() == 0){
        lastdir = nodir;
    }else if (swfinder() == 1 && lastdir == nodir){
        lastdir = down;
    }else if (swfinder() == 2 && lastdir == nodir){
        lastdir = up;
    }else if (swfinder() == 4 && lastdir == nodir){
        lastdir = right;
    }else if (swfinder() == 8 && lastdir == nodir){
        lastdir = left;
    }else if (swfinder() == 16){
        lastdir = dpause;
    }else{
    	return nodir;
    }
    return lastdir;
}

int draw_frogger(){
    int p;
    dir mydir = getdir();
    if(mydir == up){
        if(frog_y > (240/gridy)){
            frog_y = frog_y-(240/gridy);}
    }else if(mydir == down){
        if(frog_y < (240/gridy)*(gridy-1)){
            frog_y = frog_y+(240/gridy);}
    }else if(mydir == right){
        if(frog_x < (320/gridx)*(gridx-1)){
            frog_x = frog_x+(320/gridx);}
    }else if(mydir == left){
        if(frog_x > 0){
            frog_x = frog_x-(320/gridx);}
    }else if (mydir == dpause){
        p = pause();
        if (p == 0) return 0;
        if (p == 1) return 1;
        if (p == 2) {
            savegame();
            return 1;
        }
    }
    //Draw's Frogger
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x,    frog_y,   frog_x+(320/gridx), frog_y+(240/gridy)-1, 0xF000, 1);
    //Draw's Frogger's Eyes
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+(320/(gridx*10)),  frog_y+(240/(gridy*6)), frog_x+(320/(gridx*10))+(320/(gridx*4)),  frog_y+(240/(gridy*12)),  0x0000, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+(320/gridx)-(320/(gridx*10))-(320/(gridx*4)), frog_y+(240/(gridy*6)), frog_x+(320/gridx)-(320/(gridx*10)), frog_y+(240/(gridy*12)),  0x0000, 1);


    return 0;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


int main(){
    initilize_vga();
    for(;;) {
    	init_variables();


        menu();
    }
    return 0;
}
