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

#include "interface_sd.h"
#include "ps2.h"

//------------------------------------------------------
// Movement variables
dir lastdir;
dir secondlastdir;


//------------------------------------------------------
// BMP Frogger colour variables
dir last_real_dir = up;
char palette[768];
unsigned char temp[480];
char* palptr;
unsigned char* tempptr;
unsigned int* colourptr;
unsigned int colourarr [256];


//------------------------------------------------------
// Frog location
int last_fx = 0;
int last_fy = 0;



//------------------------------------------------------




// Reset the entire movement matrix to all zeros
void init_matrix(){
	int i, j = 0;
	for (i=0; i<gridy; i++){
		for (j=0; j<gridx; j++){
			movement_matrix[i][j] = 0;
		}
	}
}


// Provide a starting position for hard coded level design
// All are chosen by hand to fill the screen and present a challenging game
void init_variables(){

	// Initial frogger position, lives, and time
	frog_x = (320/gridx)*6;
	frog_y = (240/gridy)*9;
	lives_remaining = 3;
	time_var1 = 150;
	time_var2 =0;
	
	// Truck startings
	trucks[0] = (320/gridx)*0;
	trucks[1] = (320/gridx)*8;
	trucks[2] = (320/gridx)*4;
	trucks[3] = (320/gridx)*15;
	trucks[4] = (320/gridx)*10;
	trucks[5] = (320/gridx)*15;
	
	// Car startings
	cars[0] = (320/gridx)*1;
	cars[1] = (320/gridx)*6;
	cars[2] = (320/gridx)*11;
	cars[3] = (320/gridx)*2.5;
	cars[4] = (320/gridx)*7.5;
	cars[5] = (320/gridx)*12.5;
	cars[6]	= (320/gridx)*2;
	cars[7] = (320/gridx)*4;
	cars[8] = (320/gridx)*2;
	cars[9] = (320/gridx)*8;
	cars[10]= (320/gridx)*10;
	cars[11]= (320/gridx)*8;
	cars[12]= (320/gridx)*16;
	cars[13]= (320/gridx)*0;
	cars[14]= (320/gridx)*6;

	// log startings
	logs[0]	= (320/gridx)*1;
	logs[1]	= (320/gridx)*5;
	logs[2]	= (320/gridx)*2.5;
	logs[3]	= (320/gridx)*7.5;
	logs[4]	= (320/gridx)*3.5;
	logs[5]	= (320/gridx)*10;
}


// Helper function to determing if Frogger is standing on a WinBlock
// This returns a 1 to tell the caller that the current position is a winblock
int checkwin(){
	if (g[frog_y/(240/gridy)][frog_x/(320/gridx)] == WinBlock)
		return 1;
	else
		return 0;
}

// Switch Finder helper function
// Used to directly address the IO for switches value
// Only used in case of absent keyboard
int swfinder(){
	return IORD_8DIRECT(YOUR_SWITCHES_ADDR, 0);
}


// Get keyboard directions, or alteratively, switch directions
dir getdir(){
	if ( (keyb_set) ) {
		dir dirFromKeyB;
		dirFromKeyB = getDirectionFromKeyboard(lastdir, secondlastdir);
		if ( (dirFromKeyB == dpause) || (dirFromKeyB == up) || (dirFromKeyB == down) || (dirFromKeyB == right) || (dirFromKeyB == left) ) {
			lastdir = dirFromKeyB;
			last_real_dir = dirFromKeyB;
			return dirFromKeyB;
		} else if ( (dirFromKeyB == changedir) || (dirFromKeyB == dontchangedir) ){
			secondlastdir = dirFromKeyB;
			lastdir = nodir;
			return nodir;
		} else
			lastdir = nodir;
		return nodir;
	} else {
	
	// If there is no attached keyboard, the program will fall back to checking the switches
	// for a change in input to denote a movement.
	// last_real_dir is updated to denote the last actual direction, used for the direction frogger faces.
	// lastdir contains the nominal direction; may or may not be an actual direction.
		if       (swfinder() == 0){
			lastdir = nodir;
		}else if (swfinder() == 1 && lastdir == nodir){
			lastdir = down;
			last_real_dir = down;
		}else if (swfinder() == 2 && lastdir == nodir){
			lastdir = up;
			last_real_dir = up;
		}else if (swfinder() == 4 && lastdir == nodir){
			lastdir = right;
			last_real_dir = right;
		}else if (swfinder() == 8 && lastdir == nodir){
			lastdir = left;
			last_real_dir = left;
		}else if (swfinder() == 16){
			lastdir = dpause;
		}else{
			return nodir;
		}
		return lastdir;

	}
}


// Draw frogger to the VGA screen.
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

	frog_bmp_draw();

	return 0;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	Code break : BMP Conversion ahead
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
unsigned int get_colour(char* palptr, unsigned char tempstuff){
	unsigned int blue,green,red;
	unsigned int colour;
	blue  = *(palptr + 3*tempstuff  + 0);
	green = *(palptr + 3*tempstuff + 1);
	red   = *(palptr + 3*tempstuff + 2);

	colour = ( (red << 8)& 0xf800) | ( (green << 3) & 0x07E0 )| ( (blue >> 3 )& 0x1F);

	return colour;
}
 */

unsigned int* palgen(char* palptr){
	unsigned int blue,green,red;
	unsigned int* colour = &colourarr[0];
	int i;
	for (i = 0; i<256; i++){
		blue  = *(palptr + 3*i  + 0);
		green = *(palptr + 3*i + 1);
		red   = *(palptr + 3*i + 2);
		colourarr[i] = ( (red << 8)& 0xf800) | ( (green << 3) & 0x07E0 )| ( (blue >> 3 )& 0x1F);
	}
	return colour;
}


void frog_bmp_init(){
	palptr = &palette[0];
	tempptr = &temp[0];
	bmp_read("blank.bmp", tempptr, palptr);
	bmp_read("frogger2.bmp", tempptr, palptr);
	//bmp_read("frogger4.bmp", tempptr, palptr);
	colourptr = palgen (palptr);
}

void frog_bmp_draw(){
	int i,j;
	unsigned int colour;

	//if (last_fx != frog_x || last_fy != frog_y){
	if ( last_real_dir == up ){
		for( i=0; i<24; i++){
			for ( j=0; j<20; j++){
				colour = colourarr[temp[i*20+j]];
				if (colour != 0xffff)

					alt_up_pixel_buffer_dma_draw(backbuffer, colour,  frog_x+j, frog_y-i+23);
			}
		}
	}else if ( last_real_dir == down){
		for( i=0; i<24; i++){
			for ( j=0; j<20; j++){
				colour = colourarr[temp[i*20+j]];
				if (colour != 0xffff)
					alt_up_pixel_buffer_dma_draw(backbuffer, colour, frog_x+j, frog_y+i);
			}
		}
	}else if (last_real_dir == right){
		for( i=0; i<24; i++){
			for ( j=0; j<20; j++){
				colour = colourarr[temp[i*20+j]];
				if (colour != 0xffff)
					alt_up_pixel_buffer_dma_draw(backbuffer, colour, frog_x+i, frog_y+j+2);
			}
		}
	}else if (last_real_dir == left){
		for( i=0; i<24; i++){
			for ( j=0; j<20; j++){
				colour = colourarr[temp[i*20+j]];
				if (colour != 0xffff)
					alt_up_pixel_buffer_dma_draw(backbuffer, colour, frog_x-i+19, frog_y+j+2);
			}
		}
	}

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	Code break : Last but not least, main runnable function
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


int main(){

	// Set up all initializations
	initilize_vga();

	sd_init();

	frog_bmp_init();

	initialize_ps2();

	// If kicked out of menu, we should reset the variables and give the menu back to the user.
	for(;;) {
		level = 1;
		init_variables();

		menu();
	}

	return 0;

}
