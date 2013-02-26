#ifndef DRAW_LEVEL_H_
#define DRAW_LEVEL_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>

#include "play_game.h"


//----------------------------
// Level colour references

#define c_NoBlock   0x0000
#define c_Grass     0x0f00
#define c_Water     0x00f0
#define c_Highway   8 | (8 << 6) | (8 << 11)
#define c_WinBlock  0x0f10
#define c_Yellow	0xff00

//----------------------------
// Time variables

char time_remaining[20];
char time_r[5];

//----------------------------
// Helper variables for the 
// grass/flower proof of concept

char grass[480];
char* grassptr = &grass[0];
char palette[768];
char* palptr = &palette[0];
unsigned int colour[256];
int pghf =0;

//----------------------------



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

/*

// Old printgrid function, not including BMP images.
void printgrid(){
    int i,j = 0;

    for (i=0; i<gridy; i++){
        for (j=0; j<gridx; j++){
            if      (g[i][j] == NoBlock)
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
 */
 
// Print Grid Helper function:
// Used to load the flower into memory
// Will only be called once due to pghf flag
void pgh(){
	if (pghf ==0){
		int i;
		// Get image from SD card
		bmp_read("flower.bmp", grassptr, palptr);
		unsigned int blue,green,red;
		
		// Load colour data into the preset arrays
		for (i = 0; i<256; i++){
			blue  = *(palptr + 3*i  + 0);
			green = *(palptr + 3*i + 1);
			red   = *(palptr + 3*i + 2);
			colour[i] = ( (red << 8)& 0xf800) | ( (green << 3) & 0x07E0 )| ( (blue >> 3 )& 0x1F);
		}
		// Never return to function by setting flag
		pghf=1;
	}
}


// Print grid function:
// Prints the entire level to the screen, grid piece by grid piece.
void printgrid(){
	int i,j = 0;
	
	// Set up colour data
	pgh();
	
	// Print the entire grid onto the VGA screen
	for (i=0; i<gridy; i++){
		for (j=0; j<gridx; j++){
			// Print a block with corresponding colour, and/or image.
			
			if      (g[i][j] == NoBlock)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_NoBlock, 1);
				
			else if (g[i][j] == Grass)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Grass, 1);
				
			else if (g[i][j] == Water){
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Water, 1);
				movement_matrix[i][j] = 1;
			}
			else if (g[i][j] == CoolGrass){
				// Ensure to print pixel by pixel of the BMP for specified block
				int m,n;
				unsigned int col;
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Grass, 1);
				for( m=0; m<24; m++){
					for ( n=0; n<20; n++){
						col = colour[grass[m*20+n]];
						
						// Remove white from the BMP to provide transparency
						if (col != 0xffff)
							alt_up_pixel_buffer_dma_draw(backbuffer, col,  (320/gridx*j)+n, (240/gridy*i)-m+23);
					}
				}
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

// Setup the hardcoded level design
void setup_level(int level_select){
	int i;
	
	// Level one, highways and water
	if (level_select == 1){
		for ( i=0; i<gridx; i++ ){
			g[1][i] = WinBlock;

			g[2][i] = Water;
			g[3][i] = Water;
			g[4][i] = Water;

			g[5][i] = Grass;
			g[5][2] = CoolGrass;
			g[5][6] = CoolGrass;
			g[6][i] = Highway;
			
			// Provide a bit of line colourization to the highway
			if(i%2){
				g[7][i] = Highway;
				g[8][i] = Highway;
			}
			else{
				g[7][i] = Highway_t;
				g[8][i] = Highway_t;
			}

			g[9][i] = Grass;
			g[9][3] = CoolGrass;
		}
	}
	
	// Level two, all highways
	if(level_select == 2){
		for ( i=0; i<gridx; i++ ){
			g[1][i] = WinBlock;

			g[2][i] = Highway;

			if(i%2){
				g[3][i] = Highway;
				g[4][i] = Highway;
				g[5][i] = Highway;
				g[6][i] = Highway;
				g[7][i] = Highway;
				g[8][i] = Highway;
			}else{
				g[3][i] = Highway_t;
				g[4][i] = Highway_t;
				g[5][i] = Highway_t;
				g[6][i] = Highway_t;
				g[7][i] = Highway_t;
				g[8][i] = Highway_t;
			}

			g[9][i] = Grass;
			if (i%4 == 0) g[9][i] = CoolGrass;

		}
	}

}

#endif
