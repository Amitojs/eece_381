#ifndef MAIN_H_
#define MAIN_H_

#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"

//------------------------------------------------------
//Directions
typedef enum { up, down, left, right, nodir, dpause, changedir, dontchangedir } dir;
//------------------------------------------------------
typedef enum { NoBlock, Grass, Water, Highway, WinBlock, Highway_t } background;
background g[50][50];
int keyb_set;
char score[20];
char temp_highscore[20];

void setup_level();
void init_matrix();
void draw_topinfo();
int pause();
int win();
int playgame();
int die();
void printgrid();
void savegame();
void loadgame();
int checkwin();
int swfinder();
dir getdir();
int draw_frogger();
int highscores();
int menu();
int sd_playgame();
int win_menu();
void init_variables();
void frog_bmp_init();
void frog_bmp_draw();

dir getDirectionFromKeyboard(dir lastdir, dir secondlastdir);
void initialize_ps2 ();

#endif /* MAIN_H_ */
