#include <stdio.h>
#include <io.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>

//------------------------------------------------------\\
//F_LOCAL:

//Change this in your local S ram address in QSYS
#define YOUR_SRAM_ADDR 		0x80000

//Change this to your local SW address
#define YOUR_SWITCHES_ADDR		0x0004010

//Change to your QSYS local name
#define YOUR_PIXEL_BUFFER_NAME 	"/dev/pixel_buffer_dma"

//Change to your QSYS local name
#define YOUR_CHAR_BUFFER_NAME 	"/dev/char_drawer"

//------------------------------------------------------//





//------------------------------------------------------\\
//Global Variables

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev *char_buffer;

	int movement_matrix[10][16];
	int time_var1 = 150;
	int truck_1=0;
	int truck_2=160;

//------------------------------------------------------//


void init_matrix(){
    int i, j = 0;
	for (i=0; i<10; i++){
		for (j=0; j<16; j++){
			movement_matrix[i][j] = 0;
		}
	}
}

void initilize_vga(){

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(YOUR_PIXEL_BUFFER_NAME);

	unsigned int pixel_buffer_addr1 = YOUR_SRAM_ADDR;
	unsigned int pixel_buffer_addr2 = YOUR_SRAM_ADDR + (320 * 240 * 2);

	// Set the 1st buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);

	// Swap buffers – we have to swap because there is only an API function
	// to set the address of the background buffer.
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

	// Clear both buffers (this makes all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	// Initialize
	char_buffer = alt_up_char_buffer_open_dev(YOUR_CHAR_BUFFER_NAME);
	alt_up_char_buffer_init(char_buffer);

	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_char_buffer_clear(char_buffer);
}

void draw_background(){
    //Finish
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 24, 320, 48, 0x0F00, 1);
    //Water
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 48, 320, 120, 0x00F0, 1);
    //Middle
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 120, 320, 144, 0x0F00, 1);
    //Road
    unsigned grey = 8 | (8 << 6) | (8 << 11);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 144, 320, 216, grey, 1);
    //Start
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 216, 320, 240, 0x0F00, 1);

    alt_up_char_buffer_string(char_buffer, "Time Remaining: 0:60", 0, 0);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 90, 0, 150, 3, 0x0F00, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 150, 0, time_var1, 3, 0xFFFF, 1);
    alt_up_char_buffer_string(char_buffer, "Lives Remaining: 3", 0, 2);

}

void trucks_test(void){

	//Truck 1
		if(truck_1 >= 20){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-20, 192, truck_1, 215, 0xFF00, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1, 215, 0xFF00, 1);
		}
		if(truck_1 >= 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-40, 192, truck_1-20, 215, 0xFFFF, 1);
		}else if(truck_1 > 20 && truck_1 < 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1-20, 215, 0xFFFF, 1);
		}
		if (truck_1 >= 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-60, 192, truck_1-40, 215, 0xFFFF, 1);
		}else if(truck_1 > 40 && truck_1 < 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1-40, 215, 0xFFFF, 1);
		}

		truck_1=truck_1+1;
		if(truck_1 == 380){truck_1=0;}
		movement_matrix[192/24][(truck_1)/20] = 1;
		if (truck_1 >= 20){movement_matrix[192/24][(truck_1-20)/20] = 1;}
		if (truck_1 >= 40){movement_matrix[192/24][(truck_1-40)/20] = 1;}
	//Truck 2
		if(truck_2 >= 20){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-20, 192, truck_2, 215, 0xFF00, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2, 215, 0xFF00, 1);
		}
		if(truck_2 >= 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-40, 192, truck_2-20, 215, 0xFFFF, 1);
		}else if(truck_2 > 20 && truck_2 < 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2-20, 215, 0xFFFF, 1);
		}
		if (truck_2 >= 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-60, 192, truck_2-40, 215, 0xFFFF, 1);
		}else if(truck_2 > 40 && truck_2 < 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2-40, 215, 0xFFFF, 1);
		}

		truck_2=truck_2+1;
		if(truck_2 == 380){
			truck_2=0;
		}

		movement_matrix[192/24][(truck_2)/20] = 1;
		if (truck_2 >= 20){movement_matrix[192/24][(truck_2-20)/20] = 1;}
		if (truck_2 >= 40){movement_matrix[192/24][(truck_2-40)/20] = 1;}

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

int main(){

	initilize_vga();

//Initilize Variables


	int x;
	int time_var2 =0;
	int frog_x = 120;
	int frog_y = 216;
	int sw0;
	int i,j =0;

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);


	while(1){

        init_matrix();
        draw_background();
		trucks_test();


//START - Frogger
		sw0 = IORD_8DIRECT(YOUR_SWITCHES_ADDR, 0);
		if(sw0 == 1){
			frog_y = frog_y-24;
			for(x=0; x<1000000; x++){}
		}
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x, frog_y, frog_x+20, frog_y+24, 0xF000, 1);
//FINISH - Frogger


		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);


//START - Collision
		if(movement_matrix[frog_y/24][frog_x/20] == 1){
			//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
			alt_up_char_buffer_string(char_buffer, "There Was A Collision", 30, 30);
			return 0;
		}
//FINISH - Collision

//Time
		if (time_var2 <= 60){
			++time_var2;
			if (time_var2 >=60 && time_var1 >= 90){
				--time_var1;
				time_var2=0;
			}
		}

	}
	return 0;
}
