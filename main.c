#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "gba.h"
#include "images/background.h"
#include "images/wasted.h"
#include "images/rocket.h"
#include "images/alien.h"


void startState(void);

int main(void) {

  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  struct state currentState, previousState;
  struct player player = {INIT_ROW, INIT_COL};
  currentState.gamestate = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons
    previousState = currentState;
    UNUSED(previousState);
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      currentState.gamestate = START;
    }
    switch (currentState.gamestate) {
      case START:
        break;


      case HOLD:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          currentState.gamestate = INIT_PLAY;
        }
        currentState.player = player;
        createEnemy(currentState.enemies);
        break;

      case INIT_PLAY:
        currentState.score = 0;
        currentState.player = player;
        createEnemy(currentState.enemies);
        currentState.gamestate = PLAY;
        break;


    case PLAY:
      if (KEY_DOWN(BUTTON_UP, currentButtons)) {
        currentState.player.row -=  2;
      }
      if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
        currentState.player.row +=  2;
      }
      if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        currentState.player.col +=  2;
      }
      if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        currentState.player.col -=  2;
      }
      if (currentState.player.row >= HEIGHT - 35) {
        currentState.player.row = HEIGHT - 35;
      } else if (currentState.player.row <= 0) {
        currentState.player.row = 0;
      }
      if (currentState.player.col >= WIDTH - 13) {
        currentState.player.col = WIDTH - 13;
      } else if (currentState.player.col <= 0 ) {
        currentState.player.col = 0;
      }
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (currentState.enemies[i].vdir != 0) {
          currentState.enemies[i].row += (1 + (currentState.score / 1000)) * currentState.enemies[i].vdir;
          if (currentState.enemies[i].row >= currentState.enemies[i].max_row) {
            currentState.enemies[i].row = currentState.enemies[i].max_row;
            currentState.enemies[i].vdir = -1;
          } else if (currentState.enemies[i].row <= currentState.enemies[i].min_row) {
            currentState.enemies[i].row = currentState.enemies[i].min_row;
            currentState.enemies[i].vdir = 1;
          }
        }
        if (currentState.enemies[i].hdir != 0) {
          currentState.enemies[i].col += (1 + (currentState.score / 1000)) * currentState.enemies[i].hdir;
          if (currentState.enemies[i].col >= currentState.enemies[i].max_col) {
            currentState.enemies[i].col = currentState.enemies[i].max_col;
            currentState.enemies[i].hdir = -1;
          } else if (currentState.enemies[i].col <= currentState.enemies[i].min_col) {
            currentState.enemies[i].col = currentState.enemies[i].min_col;
            currentState.enemies[i].hdir = 1;
          }
        }
      }
      break;


      case LOSE:
        break;


      case HOLD_LOSE:
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
        currentState.gamestate = INIT_PLAY;
      }
        break;
    }


    waitForVBlank();
    switch (currentState.gamestate) {
      case START:
        drawFullScreenImageDMA(background);
        drawCenteredString(50, WIDTH/2 - 40, 80, 20, "DONT LET THE ALIENS CATCH YOU!", RED);
        drawCenteredString(70, WIDTH/2 - 30, 60, 20, "PRESS START", RED);
        currentState.gamestate = HOLD;
        break;


      case HOLD:
        break;


      case INIT_PLAY:
        break;


      case PLAY:
        waitForVBlank();
        fillScreenDMA(BLACK);
        drawImageDMA(currentState.player.row, currentState.player.col, 13, 35, rocket);
        drawImageDMA(currentState.enemies[0].row, currentState.enemies[0].col, 20, 20, alien);
        if (checkCollision(currentState.player, currentState.enemies[0])) {
          currentState.gamestate = LOSE;
          break;
        }
        char scoreString[9];
        sprintf(scoreString, "Score: %d", currentState.score);
        drawString(150, 5, scoreString, RED);
        currentState.score++;
        break;


      case LOSE:
        drawFullScreenImageDMA(wasted);
        drawCenteredString(50, WIDTH/2 - 40, 80, 20, scoreString, RED);
        drawCenteredString(100, WIDTH/2 - 30, 60, 20, "PRESS START TO PLAY AGAIN", RED);
        currentState.score = 0;
        currentState.gamestate = HOLD_LOSE;
        break;


      case HOLD_LOSE:
        break;
    }
    previousButtons = currentButtons;
  }
  UNUSED(previousButtons);
  return 0;
}

void createEnemy(struct enemy *enemies) {
  enemies[0].row = 0;
  enemies[0].col = 0;
  enemies[0].hdir = 1;
  enemies[0].vdir = 1;
  enemies[0].max_row = 140;
  enemies[0].min_row = 0;
  enemies[0].max_col = 220;
  enemies[0].min_col = 0;
}
int checkCollision(struct player player, struct enemy enemy) {
  if(player.col >= enemy.col + 20 || enemy.col >= player.col + 15) {
    return 0;
  }
  if(player.row + 30 <= enemy.row || enemy.row + 20 <= player.row) {
    return 0;
  }
  return 1;
}
