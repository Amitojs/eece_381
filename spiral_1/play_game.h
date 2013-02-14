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
#include "interface_SD.h"



//------------------------------------------------------
// Global Variables

int frog_x;
int frog_y;
int time_var1;
int time_var2;
int lives_remaining;
int highscore = 0;

#define numtrucks 20
int trucks[numtrucks];

//int truck_1,truck_2;

#define numcars 20
int cars[numcars];

#define numlogs 20
int logs[numlogs];
int logsdir[numlogs];

int logsy[numlogs];
int carsy[numcars];
int trucksy[numtrucks];

int level = 1;
//------------------------------------------------------
int sd_playgame(){

	int i,j;
	int countlog=0;
	int countcar=0;
	int counttruck=0;
	char levelbuffer[10][16];
	char objectbuffer [10][16];
	char* levelptr;
	char* objectptr;

	levelptr = &levelbuffer[0][0];
	objectptr = &objectbuffer[0][0];
	level_data("level1.txt",levelptr, objectptr);

	for (i=0; i<10; i++){
		for (j=0; j<16; j++){
			if ((unsigned char)levelbuffer[i][j] == '0')
				g[i][j] = NoBlock;
			else if ((unsigned char)levelbuffer[i][j] == '1')
				g[i][j] = Grass;
			else if ((unsigned char)levelbuffer[i][j] == '2')
				g[i][j] = Water;
			else if ((unsigned char)levelbuffer[i][j] == '3')
				g[i][j] = Highway;
			else if ((unsigned char)levelbuffer[i][j] == '4')
				g[i][j] = WinBlock;
			printf("%c", levelbuffer[i][j]);
		}
	}
	countlog=0;
	countcar=0;
	counttruck=0;

	for (i=0; i<10; i++){
		for (j=0; j<16; j++){
			if ( objectbuffer[i][j] == '3'){
				logs[countlog] = (320/gridx)*j;
				logsdir[countlog] = i%2;
				logsy[countlog++] = (240/gridy)*i;
			}
			else if ( objectbuffer[i][j] == '2'){
				cars[countcar] = (320/gridx)*j;
				carsy[countcar++] = (240/gridy)*i;
			}
			else if ( objectbuffer[i][j] == '1'){
				trucks[counttruck] = (320/gridx)*j;
				trucksy[counttruck++] = (240/gridy)*i;
			}
			else if ( objectbuffer[i][j] == '4'){
				frog_x = (320/gridx)*j;
				frog_y = (240/gridy)*i;
			}
		}
	}




	for(;;){
		init_matrix();
		printgrid();
		draw_topinfo();

		for (i = 0; i<countlog; i++){
			logs[i]	= draw_vehicle(log,   logs[i],   logsy[i], 0x5200,  3,   logsdir[i], 1);
		}
		for (i = 0; i<countcar; i++){
			cars[i]	= draw_vehicle(car,   cars[i],   carsy[i], 0x867,  2,   1, 2);
		}
		for (i = 0; i<counttruck; i++){
			trucks[i]	= draw_vehicle(truck, trucks[i], trucksy[i], 0xff0,  3,   1, 2);
		}


		if ( draw_frogger() ) {
			backbuffer = pixel_buffer;
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
			return 0;
		}

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
			win();
			if ((time_var1-90)*(lives_remaining) > highscore){
				highscore = (time_var1-90)*(lives_remaining);
			}
			printf("won");
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
		backbuffer = pixel_buffer;
		while (alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer) != 0){};
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer) != 0){};
	}

}

int playgame(){

	//Initialize Variables
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	if (level == 1){
		alt_up_char_buffer_string(char_buffer, "Level 1", 30, 30);
	}else if(level == 2){
		alt_up_char_buffer_string(char_buffer, "Level 2", 30, 30);
	}else{
		alt_up_char_buffer_string(char_buffer, "Level Error", 30, 30);
	}
	int i;
	for(i=0; i<10000000; i++){}
	alt_up_char_buffer_clear(char_buffer);

	setup_level(level);




	while(1){

		init_matrix();

		printgrid();
		draw_topinfo();

		if (level == 2 ){
			trucks[0] = draw_vehicle(truck, trucks[0], (240/gridy)*8, 0x678,  3,  1, 4);
			trucks[1] = draw_vehicle(truck, trucks[1], (240/gridy)*8, 0x534,  3,  1, 4);

			trucks[2] = draw_vehicle(truck, trucks[2], (240/gridy)*4, 0x678,  3,  1, 4);
			trucks[3] = draw_vehicle(truck, trucks[3], (240/gridy)*3, 0x534,  3,  -1, 3);
			trucks[4] = draw_vehicle(truck, trucks[4], (240/gridy)*4, 0x534,  3,  1, 4);
			trucks[5] = draw_vehicle(truck, trucks[5], (240/gridy)*4, 0x534,  3,  1, 4);


			cars[6] = draw_vehicle(car,   cars[6],   (240/gridy)*2, 0xff00,  2,  1, 5);
			cars[7] = draw_vehicle(car,   cars[7],   (240/gridy)*2, 0xff00,  2,  1, 5);

			cars[8] = draw_vehicle(car,   cars[8],   (240/gridy)*5, 0x533,  2, -1, 3);

			cars[9] = draw_vehicle(car,   cars[9],   (240/gridy)*2, 0xff00,  2,  1, 5);
			cars[10] = draw_vehicle(car,  cars[10],  (240/gridy)*2, 0xff00,  2,  1, 5);
			cars[11] = draw_vehicle(car,  cars[11],  (240/gridy)*5, 0x267,   2, -1, 3);
			cars[12] = draw_vehicle(car,  cars[12],  (240/gridy)*5, 0x267,   2, -1, 3);

			cars[13] = draw_vehicle(car,  cars[13],  (240/gridy)*3, 0x533,   2, -1, 3);
			cars[14] = draw_vehicle(car,  cars[14],  (240/gridy)*3, 0x533,   2, -1, 3);

			cars[0] = draw_vehicle(car,   cars[0],   (240/gridy)*7, 0x267,  2, -1, 3);
			cars[1] = draw_vehicle(car,   cars[1],   (240/gridy)*7, 0x234,  2, -1, 3);
			cars[2] = draw_vehicle(car,   cars[2],   (240/gridy)*7, 0x533,  2, -1, 3);
			cars[3] = draw_vehicle(car,   cars[3],   (240/gridy)*6, 0x867,  2,  1, 3);
			cars[4] = draw_vehicle(car,   cars[4],   (240/gridy)*6, 0x165,  2,  1, 3);
			cars[5] = draw_vehicle(car,   cars[5],   (240/gridy)*6, 0x378,  2,  1, 3);

		}else if (level == 1){
			trucks[0] = draw_vehicle(truck, trucks[0], (240/gridy)*8, 0x678,  3,  1, 1);
			trucks[1] = draw_vehicle(truck, trucks[1], (240/gridy)*8, 0x534,  3,  1, 1);

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
		}



		//If the movement function returns a 1, that means user quit the game
		if ( draw_frogger() ) {
			backbuffer = pixel_buffer;
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
			return 0;
		}

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
		backbuffer = pixel_buffer;
		while (alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer) != 0){};
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer) != 0){};

	}
	return 0;
}



#endif
