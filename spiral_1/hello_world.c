#include <stdio.h>
#include <io.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>


//------------------------------------------------------
// Local NIOS numbers:

//Change this in your local S ram address in QSYS
#define YOUR_SRAM_ADDR          0x80000

//Change this to your local SW address
//#define YOUR_SWITCHES_ADDR        0x2000  //Scott's
#define YOUR_SWITCHES_ADDR      0x4010  //Amitoj's

//Change to your QSYS local name
//#define YOUR_PIXEL_BUFFER_NAME    "/dev/video_pixel_buffer_dma_0"  //Scott's
#define YOUR_PIXEL_BUFFER_NAME  "/dev/pixel_buffer_dma"  //Amitoj's

//Change to your QSYS local name
#define YOUR_CHAR_BUFFER_NAME   "/dev/char_drawer"

//Directions
typedef enum { up, down, left, right, nodir, dpause } dir;

//------------------------------------------------------




//------------------------------------------------------
// Level design
typedef enum { NoBlock, Grass, Water, Highway, WinBlock } background;
#define c_NoBlock   0x0000
#define c_Grass     0x0f00
#define c_Water     0x00f0
#define c_Highway   8 | (8 << 6) | (8 << 11)
#define c_WinBlock  0x0f10

#define gridx 16
#define gridy 10

int pause();
int win();
int mainrun();
int die();
//------------------------------------------------------





//------------------------------------------------------
// Global Variables

    alt_up_pixel_buffer_dma_dev* pixel_buffer;
    alt_up_char_buffer_dev *char_buffer;

    int issavegame = 0;
    int movement_matrix[10][16];
    int time_var1 = 150;
    background g[10][16];
    dir lastdir;
    int frog_x = 120;
    int frog_y = 216;
    int lives_remaining = 3;
    int time_var2 =0;
    int truck_1 = 0;
    int truck_2 = 160;
    int car_1 = 20;
    int car_2 = 120;
    int car_3 = 220;
    int car_4 = 50;
    int car_5 = 150;
    int car_6 = 250;
    int log_1 = 20;
    int log_2 = 100;
    int log_3 = 50;
    int log_4 = 150;
    int log_5 = 70;
    int log_6 = 200;


//-------------------------
//    Save variables

    int s_time_var1;
    int s_frog_x;
    int s_frog_y;
    int s_lives_remaining;
    int s_time_var2;
    int s_truck_1;
    int s_truck_2;
    int s_car_1;
    int s_car_2;
    int s_car_3;
    int s_car_4;
    int s_car_5;
    int s_car_6;
    int s_log_1;
	int s_log_2;
	int s_log_3;
	int s_log_4;
	int s_log_5;
	int s_log_6;

//------------------------------------------------------




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


void draw_topinfo(){
    alt_up_char_buffer_string(char_buffer, "Time Remaining: 0:60", 0, 0);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 90, 0, 150, 3, 0x0F00, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 150, 0, time_var1, 3, 0xFFFF, 1);
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
    if (truck >= 60){movement_matrix[y_location/24][(truck-60)/20] = 1;}
    return truck;
}

//car is the starting horizontal location
//y_location is the constant starting vertical location
//color is the color of the car
//direction is the direction the car moves in, 1 is to the right, and -1 is to the left
int draw_car(int car, int y_location, int colour, int direction){
    switch(direction){
        case 1:
            if(car>= 20){
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, car-20, y_location, car, y_location+23, colour, 1);
            }else{
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, car, y_location+23, colour, 1);
            }
            if(car>= 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, car-40, y_location, car-20, y_location+23, colour, 1);
            }else if(car> 20 && car< 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, car-20, y_location+23, colour, 1);
            }
            car=car+2;


            if(car >= 380){car=0;}
            movement_matrix[y_location/24][(car)/20] = 1;
            if (car>= 20){movement_matrix[y_location/24][(car-20)/20] = 1;}
            if (car>= 40){movement_matrix[y_location/24][(car-40)/20] = 1;}
            break;
        default:
            if(car >= 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, car-40, y_location, car-20, y_location+23, colour, 1);
            }else if(car > 20 && car < 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, car-20, y_location+23, colour, 1);
            }
            if(car >= 20){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, car-20, y_location, car, y_location+23, colour, 1);
            }else{
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, car, y_location+23, colour, 1);
            }
            car=car-2;
            if(car <= 0){car=360;}
            movement_matrix[y_location/24][(car)/20] = 1;
            if (car>= 20){movement_matrix[y_location/24][(car-20)/20] = 1;}
            if (car>= 40){movement_matrix[y_location/24][(car-40)/20] = 1;}
            break;
            }

    return car;
}

int draw_log(int log, int y_location, int colour, int size, int direction){
    switch(direction){
        case 1:
            if(log>= 20){
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-20, y_location, log, y_location+23, colour, 1);
            }else{
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log, y_location+23, colour, 1);
            }
            if(log>= 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-40, y_location, log-20, y_location+23, colour, 1);
            }else if(log> 20 && log< 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log-20, y_location+23, colour, 1);
            }
            if (log >= 60 && size == 3){
            	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-60, y_location, log-40, y_location+23, colour, 1);
            }else if(log > 40 && log < 60 && size == 3){
            	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log-40, y_location+23, colour, 1);
            }

            log=log+1;
            if(log >= 380){log=0;}
            movement_matrix[y_location/24][(log)/20] = 0;
            if (log>= 20){movement_matrix[y_location/24][(log-20)/20] = 0;}
            if (log>= 40){movement_matrix[y_location/24][(log-40)/20] = 0;}
            if (log >= 60 && size == 3){movement_matrix[y_location/24][(log-60)/20] = 0;}
            break;
        default:
            if(log >= 20){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-20, y_location, log, y_location+23, colour, 1);
            }else{
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log, y_location+23, colour, 1);
            }
            if(log >= 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-40, y_location, log-20, y_location+23, colour, 1);
            }else if(log > 20 && log < 40){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log-20, y_location+23, colour, 1);
            }
            if(log >= 60 && size == 3){
				 alt_up_pixel_buffer_dma_draw_box(pixel_buffer, log-60, y_location, log-40, y_location+23, colour, 1);
			 }else if(log > 40 && log < 60 && size == 3){
				 alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, y_location, log-40, y_location+23, colour, 1);
			 }
            log=log-1;
            if(log <= 0){log=380;}
            movement_matrix[y_location/24][(log)/20] = 0;
            if (log>= 20){movement_matrix[y_location/24][(log-20)/20] = 0;}
            if (log>= 40){movement_matrix[y_location/24][(log-40)/20] = 0;}
            if (log>= 60 && size == 3){movement_matrix[y_location/24][(log-60)/20] = 0;}
            break;
    	}
    return log;
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

        }
    }
}

void setup_level(){
    int i;
    for ( i=0; i<16; i++ ){
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

void savegame(){
    issavegame = 1;
    //Save the status of the game to be loaded
    s_time_var1 = time_var1;
    s_frog_x = frog_x;
    s_frog_y = frog_y;
    s_lives_remaining = lives_remaining;
    s_time_var2 = time_var2;
    s_truck_1 = truck_1;
    s_truck_2 = truck_2;
    s_car_1 = car_1;
    s_car_2 = car_2;
    s_car_3 = car_3;
    s_car_4 = car_4;
    s_car_5 = car_5;
    s_car_6 = car_6;
    s_log_1 = log_1;
	s_log_2 = log_2;
	s_log_3 = log_3;
	s_log_4 = log_4;
	s_log_5 = log_5;
	s_log_6 = log_6;
}

void loadgame(){
    int i;

    //Check save game
    if (issavegame == 0){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
        alt_up_char_buffer_string(char_buffer, "No game available.", 30, 30);
        for (i=0; i<10000000; i++);
    }else{
        time_var1 = s_time_var1;
        frog_x = s_frog_x;
        frog_y = s_frog_y;
        lives_remaining = s_lives_remaining;
        time_var2 = s_time_var2;
        truck_1 = s_truck_1;
        truck_2 = s_truck_2;
        car_1 = s_car_1;
        car_2 = s_car_2;
        car_3 = s_car_3;
        car_4 = s_car_4;
        car_5 = s_car_5;
        car_6 = s_car_6;
        log_1 = s_log_1;
        log_2 = s_log_2;
        log_3 = s_log_3;
        log_4 = s_log_4;
        log_5 = s_log_5;
        log_6 = s_log_6;

        mainrun();
    }
}

int checkwin(){
   if (g[frog_y/24][frog_x/20] == WinBlock)
       return 1;
   else
       return 0;
}

int swfinder(){
    return IORD_8DIRECT(YOUR_SWITCHES_ADDR, 0);
}

dir getdir(){
    if       (swfinder() == 0){
        lastdir = nodir;
        return nodir;
    }else if (swfinder() == 1 && lastdir == nodir){
        lastdir = down;
        return down;
    }else if (swfinder() == 2 && lastdir == nodir){
        lastdir = up;
        return up;
    }else if (swfinder() == 4 && lastdir == nodir){
        lastdir = right;
        return right;
    }else if (swfinder() == 8 && lastdir == nodir){
        lastdir = left;
        return left;
    }else if (swfinder() == 16){
        lastdir = dpause;
        return dpause;
    }else{
        return nodir;
    }
}

int draw_frogger(){
    int p;
    dir mydir = getdir();
    if(mydir == up){
        if(frog_y > 24){
            frog_y = frog_y-24;}
    }else if(mydir == down){
        if(frog_y < 216){
            frog_y = frog_y+24;}
    }else if(mydir == right){
        if(frog_x < 300){
            frog_x = frog_x+20;}
    }else if(mydir == left){
        if(frog_x > 0){
            frog_x = frog_x-20;}
    }else if (mydir == dpause){
        p = pause();
        if (p == 0) return 0;
        if (p == 1) return 1;
        if (p == 2) {
            savegame();
            return 1;
        }
    }
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x,    frog_y,   frog_x+20, frog_y+23, 0xF000, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+2,  frog_y+4, frog_x+7,  frog_y+6,  0x0000, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+13, frog_y+4, frog_x+18, frog_y+6,  0x0000, 1);

    return 0;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int mainrun(){

//Initilize Variables


    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

    setup_level();

    while(1){

        init_matrix();

        //draw_background();
        printgrid();
        draw_topinfo();

        truck_1 = draw_truck(truck_1, 192, 1);
        truck_2 = draw_truck(truck_2, 192, 1);
        car_1 = draw_car(car_1,  168, 0x267, -1);
        car_2 = draw_car(car_2,  168, 0x234, -1);
        car_3 = draw_car(car_3,  168, 0x533, -1);
        car_4 = draw_car(car_4,  144, 0x867,  1);
        car_5 = draw_car(car_5,  144, 0x165,  1);
        car_6 = draw_car(car_6,  144, 0x378,  1);
        log_1 = draw_log(log_1,  96,  0x5200,  3,  1);
        log_2 = draw_log(log_2,  96,  0x5200,  2,  1);
        log_3 = draw_log(log_3,  72,  0x5200,  3, -1);
        log_4 = draw_log(log_4,  72,  0x5200,  2, -1);
        log_5 = draw_log(log_5,  48,  0x5200,  3,  1);
        log_6 = draw_log(log_6,  48,  0x5200,  3,  1);


        //If the movment function returns a 1, that means user quit the game
        if ( draw_frogger() ) {
            alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
            return 0;
        }

        while (alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer) != 0){};
        while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer) != 0){};

//START - Collision
        if(movement_matrix[frog_y/24][frog_x/20] == 1 ){
            //alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
            frog_x = 120;
            frog_y = 216;
            --lives_remaining;
            if(lives_remaining == 0){
                die();
                return 0;}
        }
//FINISH - Collision

        if ( checkwin() ) {
            win();
            //save score
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

int highscores(){
    int i;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    alt_up_char_buffer_string(char_buffer, "High Scores unavailable.", 30, 30);
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
    alt_up_char_buffer_string(char_buffer, "YOU LOSE!", 30, 30);
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
                mainrun();
            }else if (sel%3 == 1){
                alt_up_char_buffer_clear(char_buffer);
                highscores();
            }else if (sel%3 == 2){
                alt_up_char_buffer_clear(char_buffer);
                loadgame();
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


int main(){
    initilize_vga();
    for(;;) {
        menu();
        frog_x = 120;
        frog_y = 216;
        lives_remaining = 3;
        time_var1 = 150;
        time_var2 =0;
        truck_1 = 0;
        truck_2 = 160;
        car_1 = 20;
        car_2 = 120;
        car_3 = 220;
        car_4 = 50;
        car_5 = 150;
        car_6 = 250;
        log_1 = 20;
		log_2 = 100;
		log_3 = 50;
		log_4 = 150;
		log_5 = 70;
		log_6 = 200;

        // Clean up: put global variables back to starting values
        // or else we restart the game dead
    }
    return 0;
}
