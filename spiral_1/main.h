
#ifndef MAIN_H_
#define MAIN_H_

//------------------------------------------------------
//Directions
typedef enum { up, down, left, right, nodir, dpause } dir;
//------------------------------------------------------

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

#endif /* MAIN_H_ */
