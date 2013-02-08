#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>


//------------------------------------------------------
//Directions
typedef enum { up, down, left, right, nodir, dpause } dir;
//------------------------------------------------------



//------------------------------------------------------
// Level design
typedef enum { NoBlock, Grass, Water, Highway, WinBlock, Highway_t } background;
#define c_NoBlock   0x0000
#define c_Grass     0x0f00
#define c_Water     0x00f0
#define c_Highway   8 | (8 << 6) | (8 << 11)
#define c_WinBlock  0x0f10
#define c_Yellow	0xff00

int pause();
int win();
int playgame();
int die();
//------------------------------------------------------





//------------------------------------------------------
// Global Variables

    alt_up_pixel_buffer_dma_dev* pixel_buffer;
    alt_up_char_buffer_dev *char_buffer;

    int issavegame = 0;
    background g[50][50];
    dir lastdir;

    char time_remaining[20];
    char time_r[5];
    char score[20];
    char temp_highscore[20];


//-------------------------
//    Save variables

    int s_time_var1;
    int s_frog_x;
    int s_frog_y;
    int s_lives_remaining;
    int s_time_var2;
    int s_truck_1,	s_truck_2;

    int s_cars[numcars];
	int s_logs[numlogs];

//------------------------------------------------------




void init_matrix(){
    int i, j = 0;
    for (i=0; i<gridy; i++){
        for (j=0; j<gridx; j++){
            movement_matrix[i][j] = 0;
        }
    }
}

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
            else if (g[i][j] == Highway_t){
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i+1), c_Highway, 1);
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (320/gridx)*(j), (240/gridy)*(i), (320/gridx)*(j+1), (240/gridy)*(i)+1, c_Yellow, 1);
            }

        }
    }
}

void setup_level(){
    int i;
    for ( i=0; i<gridx; i++ ){
        g[0][i] = NoBlock;
        g[1][i] = WinBlock;

        g[2][i] = Water;
        g[3][i] = Water;
        g[4][i] = Water;

        g[5][i] = Grass;
        	g[5][0] = Water;
        	g[5][gridx-1] = Water;
        g[6][i] = Highway;

        	if(i%2){
        		g[7][i] = Highway;
        		g[8][i] = Highway;
        	}
        	else{
        		g[7][i] = Highway_t;
        		g[8][i] = Highway_t;
        	}

        g[9][i] = Grass;
        g[10][i] = Grass;
        g[11][i] = Grass;
        g[12][i] = Grass;
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

int checkwin(){
   if (g[frog_y/(240/gridy)][frog_x/(320/gridx)] == WinBlock){
	   return 1;
   }
   else
       return 0;
}

int swfinder(){
    return IORD_8DIRECT(YOUR_SWITCHES_ADDR, 0);
}

dir getdir(){
    if       (swfinder() == 0){
        lastdir = nodir;
    }else if (swfinder() == 1 && lastdir == nodir){
        lastdir = down;
    }else if (swfinder() == 2 && lastdir == nodir){
        lastdir = up;
    }else if (swfinder() == 4 && lastdir == nodir){
        lastdir = right;
    }else if (swfinder() == 8 && lastdir == nodir){
        lastdir = left;
    }else if (swfinder() == 16){
        lastdir = dpause;
    }else{
    	return nodir;
    }
    return lastdir;
}

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
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x,    frog_y,   frog_x+(320/gridx), frog_y+(240/gridy)-1, 0xF000, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+2,  frog_y+4, frog_x+7,  frog_y+6,  0x0000, 1);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, frog_x+13, frog_y+4, frog_x+18, frog_y+6,  0x0000, 1);

    return 0;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


int highscores(){
    int i;
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0x0000, 0);
    if (highscore == 0){
    	alt_up_char_buffer_string(char_buffer, "High Scores unavailable.", 30, 30);
    }else{
    	sprintf(temp_highscore, "High Score: %d", highscore);
    	alt_up_char_buffer_string(char_buffer, temp_highscore, 30, 30);
    }
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
    if (time_var1 == 90){
    	alt_up_char_buffer_string(char_buffer, "TIMES UP! YOU LOSE!", 30, 30);
    }else{
    	alt_up_char_buffer_string(char_buffer, "YOU LOSE!", 30, 30);}

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
                playgame();
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

    	frog_x = (320/gridx)*6;
    	frog_y = (240/gridy)*9;
    	lives_remaining = 3;
    	time_var1 = 150;
    	time_var2 =0;
    	truck_1 = (320/gridx)*0;
    	truck_2 = (320/gridx)*8;

    	cars[0] = (320/gridx)*1;
    	cars[1] = (320/gridx)*6;
    	cars[2] = (320/gridx)*11;
    	cars[3] = (320/gridx)*2.5;
    	cars[4] = (320/gridx)*7.5;
    	cars[5] = (320/gridx)*12.5;

    	logs[0]	= (320/gridx)*1;
    	logs[1]	= (320/gridx)*5;
    	logs[2]	= (320/gridx)*2.5;
    	logs[3]	= (320/gridx)*7.5;
    	logs[4]	= (320/gridx)*3.5;
    	logs[5]	= (320/gridx)*10;

        menu();
    }
    return 0;
}
