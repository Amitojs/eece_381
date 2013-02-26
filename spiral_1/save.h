#ifndef SAVE_H_
#define SAVE_H_

#include "main.h"
//-------------------------
//    Save variables

    int issavegame = 0;
	
    int s_level;
    int s_time_var1;
    int s_frog_x;
    int s_frog_y;
    int s_lives_remaining;
    int s_time_var2;

    int s_trucks[numtrucks];
    int s_cars[numcars];
	int s_logs[numlogs];

//-------------------------


// Defines the process that the program should take to load a saved game.
void loadgame(){
    int i;

    // Check the flag for an existing save game
    if (issavegame == 0){
		// Display that a savegame is absent
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
        alt_up_char_buffer_string(char_buffer, "No game available.", 30, 30);
        for (i=0; i<10000000; i++);
    }else{
		// Move save variables into the working variables
        time_var1 = s_time_var1;
        frog_x = s_frog_x;
        frog_y = s_frog_y;
        level = s_level;
        lives_remaining = s_lives_remaining;
        time_var2 = s_time_var2;
        trucks[0] = s_trucks[0];
        trucks[1] = s_trucks[1];

        for (i = 0; i <= numcars; i++)
            cars[i] = s_cars[i];
        for (i = 0; i <= numlogs; i++)
            logs[i] = s_logs[i];
		
		// Start the game with the loaded parameters
        playgame();
    }
}

// Defines the process to save working variables into a saved state
void savegame(){
	int i;
	
	// Set the flag to enable loading
    issavegame = 1;
	
    //Save the status of the game to be loaded from working variables
    s_time_var1 = time_var1;
    s_frog_x = frog_x;
    s_frog_y = frog_y;
    s_lives_remaining = lives_remaining;
    s_time_var2 = time_var2;
    s_level = level;
    s_trucks[0] = trucks[0];
    s_trucks[1] = trucks[1];

    for (i = 0; i <= numcars; i++)
    	s_cars[i] = cars[i];
	for (i = 0; i <= numlogs; i++)
    	s_logs[i] = logs[i];
}

#endif /* SAVE_H_ */
