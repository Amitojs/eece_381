#ifndef DRAW_VEHICLES_H_
#define DRAW_VEHICLES_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>
#include "initilize_vga.h"

//------------------------------------------------------
// Global Variables

typedef enum { truck, car, log } vehicle;

#define gridx 16 //16          //COPPIED THESE TWO DELETE AFTER
#define gridy 10 //10

int movement_matrix[50][50];

//------------------------------------------------------



int draw_vehicle(vehicle myvehicle, int x_location, int y_location, int colour, int size, int direction, int speed){
	int colour_2;
	int colour_3;

	if (myvehicle == truck){
		colour_2 = 0xFFFF;  //Making Trailer of Truck White
		colour_3 = 0xFFFF;	//Making Trailer of Truck White
	}else{
		colour_2 = colour;
		colour_3 = colour;
	}

	switch(direction){
	case 1:
		if(x_location>= (320/gridx)){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-(320/gridx), y_location, x_location, y_location+(240/gridy) - 1, colour, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location, y_location+(240/gridy) - 1, colour, 1);
		}
		if(x_location>= 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-40, y_location, x_location-(320/gridx), y_location+(240/gridy) - 1, colour_2, 1);
		}else if(x_location> (320/gridx) && x_location< 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location-(320/gridx), y_location+(240/gridy) - 1, colour_2, 1);
		}
		if (x_location >= 60 && size == 3){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-60, y_location, x_location-40, y_location+(240/gridy) - 1, colour_3, 1);
		}else if(x_location > 40 && x_location < 60 && size == 3){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location-40, y_location+(240/gridy) - 1, colour_3, 1);
		}

		x_location=x_location+speed;
		if(x_location >= (320/gridx)*(gridx+3)){x_location=0;}
		if (myvehicle == log){
			movement_matrix[y_location/(240/gridy)][(x_location)/(320/gridx)] = -1;
			if (x_location >= (320/gridx)){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx))/(320/gridx)] = -1;}
			if (x_location >= (320/gridx)*2){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx)*2)/(320/gridx)] = -1;}
			if (x_location >= (320/gridx)*3 && size == 3){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx)*3)/(320/gridx)] = -1;}
		}else{
			movement_matrix[y_location/(240/gridy)][(x_location)/(320/gridx)] = 1;
			if (x_location >= (320/gridx)){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx))/(320/gridx)] = 1;}
			if (x_location >= (320/gridx)*2){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx)*2)/(320/gridx)] = 1;}
			if (x_location >= (320/gridx)*3 && size == 3){movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx)*3)/(320/gridx)] = 1;}

		}
		break;
	default:
		if(x_location >= (320/gridx)){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-(320/gridx), y_location, x_location, y_location+(240/gridy) - 1, colour, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location, y_location+(240/gridy) - 1, colour, 1);
		}
		if(x_location >= (320/gridx)*2){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-(320/gridx)*2, y_location, x_location-(320/gridx), y_location+(240/gridy) - 1, colour_2, 1);
		}else if(x_location > (320/gridx) && x_location < (320/gridx)*2){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location-(320/gridx), y_location+(240/gridy) - 1, colour_2, 1);
		}
		if(x_location >= (320/gridx)*3 && size == 3){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x_location-(320/gridx)*3, y_location, x_location-(320/gridx)*2, y_location+(240/gridy) - 1, colour_3, 1);
		}else if(x_location > (320/gridx)*2 && x_location < (320/gridx)*3 && size == 3){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, x_location-(320/gridx)*2, y_location+(240/gridy) - 1, colour_3, 1);
		}
		x_location=x_location-speed;
		if(x_location <= 0){x_location=(320/gridx)*(gridx+2);}
		if (myvehicle == log){
			movement_matrix[y_location/(240/gridy)][(x_location)/(320/gridx)] = 0;
			if (x_location>= (320/gridx)){
				movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx))/(320/gridx)] = -2;
			}
			if (x_location>= (320/gridx)*2){
				movement_matrix[y_location/(240/gridy)][(x_location-((320/gridx)*2))/(320/gridx)] = -2;
			}
			if (x_location>= (320/gridx)*3 && size == 3){
				movement_matrix[y_location/(240/gridy)][(x_location-((320/gridx)*3))/(320/gridx)] = -2;
			}
		}else{
			if (x_location>= (320/gridx)){
				movement_matrix[y_location/(240/gridy)][(x_location-(320/gridx))/(320/gridx)] = 1;
			}
			if (x_location>= (320/gridx)*2){
				movement_matrix[y_location/(240/gridy)][(x_location-((320/gridx)*2))/(320/gridx)] = 1;
			}
			if (x_location>= (320/gridx)*3 && size == 3){
				movement_matrix[y_location/(240/gridy)][(x_location-((320/gridx)*3))/(320/gridx)] = 1;
			}
		}
		break;
	}
	return x_location;
}

#endif
