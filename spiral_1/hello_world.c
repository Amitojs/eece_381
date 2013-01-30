#include <stdio.h>
#include <io.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>


//------------------------------------------------------\\
// Local NIOS numbers:

//Change this in your local S ram address in QSYS
#define YOUR_SRAM_ADDR 			0x80000

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
	background g[10][16];

//------------------------------------------------------//



//------------------------------------------------------\\
// Level design
typedef enum { NoBlock, Grass, Water, Highway, WinBlock } background;
#define c_NoBlock 	0xffff
#define c_Grass		0x0f00
#define c_Water		0x00f0
#define c_Highway	8 | (8 << 6) | (8 << 11)
#define c_WinBlock	0x0f10

#define gridx 10
#define gridy 16
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
//truck is the starting horizontal location
//y_location is the constant starting vertical location
//direction is the direction the truck moves in, 1 is to the right, and -1 is to the left
int draw_truck(int truck, int y_location, int direction){

	if(truck >= 20){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck-20, y_location, truck, y_location+23, 0xFF00, 1);
	}else{
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, truck, y_location+23, 0xFF00, 1);
	}
	if(truck >= 40){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck-40, y_location, truck-20, y_location+23, 0xFFFF, 1);
	}else if(truck > 20 && truck < 40){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, truck-20, y_location+23, 0xFFFF, 1);
	}
	if (truck >= 60){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck-60, y_location, truck-40, y_location+23, 0xFFFF, 1);
	}else if(truck > 40 && truck < 60){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, truck-40, y_location+23, 0xFFFF, 1);
	}

	truck=truck+1;
	if(truck == 380){truck=0;}
	movement_matrix[y_location/24][(truck)/20] = 1;
	if (truck >= 20){movement_matrix[y_location/24][(truck-20)/20] = 1;}
	if (truck >= 40){movement_matrix[y_location/24][(truck-40)/20] = 1;}
	return truck;
}
void printgrid(){
	int i,j = 0;

	for (i=0; i<gridx; i++){
		for (j=1; j<gridy; j++){
			if 		(g[i][j] == NoBlock)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(i), (240/gridy)*(j), (320/gridx)*(i+1), (240/gridy)*(j+1), c_NoBlock, 0);
			else if (g[i][j] == Grass)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(i), (240/gridy)*(j), (320/gridx)*(i+1), (240/gridy)*(j+1), c_Grass, 0);
			else if (g[i][j] == Water)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(i), (240/gridy)*(j), (320/gridx)*(i+1), (240/gridy)*(j+1), c_Water, 0);
			else if (g[i][j] == Highway)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(i), (240/gridy)*(j), (320/gridx)*(i+1), (240/gridy)*(j+1), c_Highway, 0);
			else  //(g[i][j] == WinBlock)
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(i), (240/gridy)*(j), (320/gridx)*(i+1), (240/gridy)*(j+1), c_WinBlock, 0);

		}
	}
}

void setup_level(){
	int i,j;
	for ( i=0; i<10; i++ ){
		g[0][i] = NoBlock;
		g[1][i] = WinBlock;
		g[2][i] = Water;
		g[3][i] = Water;
		g[4][i] = Water;
		g[5][i] = Grass;
		g[6][i] = Highway;
		g[7][i] = Highway;
		g[8][i] = Highway;
		g[9][i] = Grass;
	}
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
	int truck_1 = 0;
	int truck_2 = 160;

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	setup_level();
	
	while(1){

        init_matrix();
		
        //draw_background();
		printgrid();
		
		truck_1 = draw_truck(truck_1, 192, 1);
		truck_2 = draw_truck(truck_2, 192, 1);


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
