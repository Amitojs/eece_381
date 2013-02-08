#ifndef MAIN_H_
#define MAIN_H_

//------------------------------------------------------
//Directions
typedef enum { up, down, left, right, nodir, dpause } dir;
//------------------------------------------------------
typedef enum { NoBlock, Grass, Water, Highway, WinBlock, Highway_t } background;
background g[50][50];

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

#endif /* MAIN_H_ */
