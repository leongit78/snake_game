#ifndef GAME_H
#define GAME_H

extern char getsymv();
extern char tolower(char);
extern void enable_raw_mode();
extern void disable_raw_mode();
int setup();
int setup_wh();
void draw();
void input();
void logic();
int get_rt();	
int check_term_valid();
void fruit_new_coords();
int fruit_is_eaten();
void tail_grow();
void tail_move();
int collision();
void check_turn();
void check_coords();
void game_loop();
void game_start();
void gameover_draw();

#endif
