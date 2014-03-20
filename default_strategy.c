#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "strategy.h"

static int (*fm)[4], dir;

static int *mp(int a, int b) {
  switch (dir) {
   case 0:
    return &fm[a][b];
   case 1:
    return &fm[b][3 - a];
   case 2:
    return &fm[3 - a][3 - b];
   case 3:
    return &fm[3 - b][a];
   default:
    return NULL;
  }
}

int move_action(int maze[4][4], int action) {
  bool flag = false;
  static int arr[4];
  static int mg[4];
  if (action < 0 || action > 3)
    return false;
  fm = maze; dir = action;
  for (int i = 0; i < 4; ++i) {
    int cc = 0;
    memset(arr, 0, sizeof arr);
    memset(mg, 0, sizeof mg);
    for (int j = 3; j >= 0; --j) {
      int t = *mp(i, j);
      if (t != 0) {
        if (cc > 0 && arr[cc - 1] == t && !mg[cc - 1]) {
          arr[cc - 1] += t;
          mg[cc - 1] = true;
          flag = true;
        } else {
          if (cc != 3 - j) flag = true;
          arr[cc++] = t;
        }
      }
    }
    for (int j = 3; j >= 0; --j)
      *mp(i, j) = arr[3 - j];
  }
  return flag;
}

int roundrobin(int maze[4][4]) {
  static int dir = -1;
  return dir = (dir + 1) % 4;
}

int roundrobin2(int maze[4][4]) {
  static int dir = 0;
  static int rmaze[4][4];
  memcpy(rmaze, maze, sizeof rmaze);
  if (move_action(rmaze, dir))
    return dir;
  else
    return dir = (dir + 1) % 4;
}

int user_input(int maze[4][4]) {
  static int imaze[4][4];
  while (true) {
    memcpy(imaze, maze, sizeof imaze);
    switch (getch()) {
     case KEY_RIGHT:
      if (move_action(imaze, 0))
        return 0;
      break;
     case KEY_DOWN:
      if (move_action(imaze, 1))
        return 1;
      break;
     case KEY_LEFT:
      if (move_action(imaze, 2))
        return 2;
      break;
     case KEY_UP:
      if (move_action(imaze, 3))
        return 3;
      break;
    }
  }
}
