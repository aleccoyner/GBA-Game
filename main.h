#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#define NUM_ENEMIES 1
#define INIT_ROW 80
#define INIT_COL 120

enum gba_state {
  START,
  HOLD,
  INIT_PLAY,
  PLAY,
  LOSE,
  HOLD_LOSE,
};

struct player {
    int row;
    int col;
};

struct enemy {
    int row;
    int col;
    int max_row;
    int min_row;
    int max_col;
    int min_col;
    int hdir;
    int vdir;
};

struct state {
    enum gba_state gamestate;
    struct player player;
    int score;
    struct enemy enemies[NUM_ENEMIES];
};

void createEnemy(struct enemy *enemies);
int checkCollision(struct player player, struct enemy enemy);

#endif
