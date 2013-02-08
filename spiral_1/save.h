#ifndef SAVE_H_
#define SAVE_H_

#include "main.h"
//-------------------------
//    Save variables

    int issavegame = 0;

    int s_time_var1;
    int s_frog_x;
    int s_frog_y;
    int s_lives_remaining;
    int s_time_var2;
    int s_truck_1,	s_truck_2;

    int s_cars[numcars];
	int s_logs[numlogs];

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

        for (i = 0; i <= numcars; i++){
            cars[i] = s_cars[i];
        }

        for (i = 0; i <= numlogs; i++){
            logs[i] = s_logs[i];
        }

        playgame();
    }
}


void savegame(){
	int i;
    issavegame = 1;
    //Save the status of the game to be loaded
    s_time_var1 = time_var1;
    s_frog_x = frog_x;
    s_frog_y = frog_y;
    s_lives_remaining = lives_remaining;
    s_time_var2 = time_var2;

    s_truck_1 = truck_1;
    s_truck_2 = truck_2;

    for (i = 0; i <= numcars; i++){
    	s_cars[i] = cars[i];
    }
	for (i = 0; i <= numlogs; i++){
    	s_logs[i] = logs[i];
    }
}

#endif /* SAVE_H_ */
