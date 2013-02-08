#ifndef PLAY_GAME_H_
#define PLAY_GAME_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>
#include "initilize_vga.h"
#include "draw_vehicles.h"
#include "main.h"



//------------------------------------------------------
// Global Variables

int frog_x;
int frog_y;
int time_var1;
int time_var2;
int lives_remaining;
int highscore = 0;

int truck_1,truck_2;

#define numcars 6
    int cars[numcars];
#define numlogs 6
    int logs[numlogs];

//------------------------------------------------------


int playgame(){

//Initialize Variables
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

    setup_level();

    while(1){

        init_matrix();

        printgrid();
        draw_topinfo();

        truck_1 = draw_vehicle(truck, truck_1, (240/gridy)*8, 0x678,  3,  1, 1);
        truck_2 = draw_vehicle(truck, truck_2, (240/gridy)*8, 0x534,  3,  1, 1);

        cars[0] = draw_vehicle(car,   cars[0],   (240/gridy)*7, 0x267,  2, -1, 2);
        cars[1] = draw_vehicle(car,   cars[1],   (240/gridy)*7, 0x234,  2, -1, 2);
        cars[2] = draw_vehicle(car,   cars[2],   (240/gridy)*7, 0x533,  2, -1, 2);
        cars[3] = draw_vehicle(car,   cars[3],   (240/gridy)*6, 0x867,  2,  1, 2);
        cars[4] = draw_vehicle(car,   cars[4],   (240/gridy)*6, 0x165,  2,  1, 2);
        cars[5] = draw_vehicle(car,   cars[5],   (240/gridy)*6, 0x378,  2,  1, 2);

        logs[0]	= draw_vehicle(log,   logs[0],   (240/gridy)*4, 0x5200,  3,   1, 1);
        logs[1]	= draw_vehicle(log,   logs[1],   (240/gridy)*4, 0x5200,  2,   1, 1);
        logs[2]	= draw_vehicle(log,   logs[2],   (240/gridy)*3, 0x5200,  3,  -1, 1);
        logs[3]	= draw_vehicle(log,   logs[3],   (240/gridy)*3, 0x5200,  2,  -1, 1);
        logs[4]	= draw_vehicle(log,   logs[4],   (240/gridy)*2, 0x5200,  3,   1, 1);
        logs[5]	= draw_vehicle(log,   logs[5],   (240/gridy)*2, 0x5200,  3,   1, 1);




        //If the movement function returns a 1, that means user quit the game
        if ( draw_frogger() ) {
            alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
            return 0;
        }

        while (alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer) != 0){};
        while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer) != 0){};

//START - Collision
        if(movement_matrix[frog_y/(240/gridy)][frog_x/(320/gridx)] == 1 ){
            frog_x = (320/gridx)*6;
            frog_y = (240/gridy)*9;
            --lives_remaining;
            if(lives_remaining == 0){
                die();
                return 0;}
        }else if (movement_matrix[frog_y/(240/gridy)][frog_x/(320/gridx)] == -1 ){
        	if (frog_x < (320/gridx)*(gridx-1) )
        		frog_x++;
        }else if (movement_matrix[frog_y/(240/gridy)][frog_x/(320/gridx)] == -2 ){
        	if (frog_x > 0 )
        		frog_x--;
        }
//FINISH - Collision

        if ( checkwin() ) {
        	win_menu();
            if ((time_var1-90)*(lives_remaining) > highscore){
            	highscore = (time_var1-90)*(lives_remaining);
            }
            return 0;
        }

//Time
        if (time_var2 <= 15){
            ++time_var2;
            if (time_var2 >=15 && time_var1 >= 90){
                --time_var1;
                if (time_var1 <= 90){
                	die();
                	return 0;
                }
                time_var2=0;
            }
        }

    }
    return 0;
}



#endif
