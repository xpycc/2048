#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "strategy.h"

#define N_BLACK   1
#define N_RED     2
#define N_GREEN   3
#define N_YELLOW  4
#define N_BLUE    5
#define N_MAGENTA 6
#define N_CYAN    7
#define N_WHITE   8

typedef int (*fun_type)(int[4][4]);
fun_type trans = next_move;

static int maze[4][4], step, maze_copy[4][4];
static short forge, back;

static const int block[11] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };
static const int color[11] = { N_BLACK, N_BLACK, N_GREEN, N_GREEN, N_CYAN, N_CYAN,
                               N_YELLOW, N_YELLOW, N_RED, N_MAGENTA, N_MAGENTA };
static const int dattr[11] = { A_BOLD, A_NORMAL, A_NORMAL, A_BOLD, A_NORMAL, A_BOLD,
                               A_NORMAL, A_BOLD, A_NORMAL, A_BOLD, A_NORMAL };

static inline void set_color_attr(int i) {
  color_set(color[i], NULL);
  attron(dattr[i]);
}

static inline void deset_color_attr(int i) {
  color_set(0, NULL);
  attroff(dattr[i]);
}

static void draw_game(char *ext_msg) {
  clear();
  move(0, 0);
  printw("Goal: ");
  set_color_attr(10);
  printw("2048");
  deset_color_attr(10);
  printw("    Steps: %d\n", step);
  char tab[2][2] = { ' ', '|', '-', '+' };
  color_set(N_BLUE, NULL); attron(A_BOLD);
  for (int i = 1; i <= 17; ++i, printw("\n"))
    for (int j = 1; j <= 33; ++j)
      printw("%c", tab[i % 4 == 1][j % 8 == 1]);
  color_set(0, NULL); attroff(A_BOLD);
  printw("Status: %s", ext_msg);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (maze[i][j] != 0) {
        move(3 + i * 4, 2 + j * 8);
        int k;
        for (k = 0; block[k] != maze[i][j]; ++k)
          ;
        set_color_attr(k);
        printw("%5d", maze[i][j]);
        deset_color_attr(k);
      }
  move(19, 0);
  refresh();
}

static int new_x, new_y;

static int fill_number(int pos, int num) {
  int spaces = 0;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) {
      if (maze[i][j] == 0 && spaces == pos) {
        maze[i][j] = num;
        new_x = i, new_y = j;
        return spaces;
      }
      spaces += maze[i][j] == 0;
    }
  return spaces;
}

static void new_number() {
  int spaces = fill_number(18, -1);
  if (spaces == 0) return;
  int num = rand() / (double)RAND_MAX < 0.1 ? 4 : 2;
  int pos = rand() / (double)RAND_MAX * spaces;
  fill_number(pos, num);
}

static void init_game() {
  srand((long)time(NULL));
  step = 0;
  memset(maze, 0, sizeof maze);
  new_number();
  draw_game("initialized.");
}

static bool check_game() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (maze[i][j] == 2048)
        return true;
  return false;
}

static bool loss_game() {
  for (int i = 0; i < 4; ++i) {
    memcpy(maze_copy, maze, sizeof maze);
    if (move_action(maze_copy, i))
      return false;
  }
  return true;
}

static const char *act_str[4] = {
  "RIGHT", "DOWN", "LEFT", "UP"
};

static void move_game(int action) {
  ++step;
  move_action(maze, action);
  new_number();

  static char buf[100];
  sprintf(buf, " move %s", act_str[action]);
  draw_game(buf);
  move(3 + new_x * 4, 2 + new_y * 8);
  color_set(N_BLUE, NULL); attron(A_BOLD);
  printw("%5d", maze[new_x][new_y]);
  color_set(0, NULL); attroff(A_BOLD);
  move(19, 0);
}

static void finish_game(bool ok) {
  if (ok) {
    // draw_game(" You win.\nPress `q' to quit, otherwise replay.\n");
    move(19, 0);
    printw("You win.\nPress `q' to quit, `c' to replay.\n");
    refresh();
  } else {
    // draw_game(" You lose.\nPress `q' to quit, otherwise replay.\n");
    move(19, 0);
    printw("You lose.\nPress `q' to quit, `c' to replay.\n");
    refresh();
  }
}

int main(int argc, char **argv) {
  // TODO read arguments
  
  if (argc > 1) {
    if (strcmp(argv[1], "-m") == 0)
      trans = user_input;
    else if (strcmp(argv[1], "-rr") == 0)
      trans = roundrobin;
    else if (strcmp(argv[1], "-rr2") == 0)
      trans = roundrobin2;
  }

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  use_default_colors();
  
  pair_content(0, &forge, &back);
  if (init_pair(N_BLACK, COLOR_BLACK, back) == ERR) return 1;
  if (init_pair(N_RED, COLOR_RED, back) == ERR) return 1;
  if (init_pair(N_GREEN, COLOR_GREEN, back) == ERR) return 1;
  if (init_pair(N_YELLOW, COLOR_YELLOW, back) == ERR) return 1;
  if (init_pair(N_BLUE, COLOR_BLUE, back) == ERR) return 1;
  if (init_pair(N_MAGENTA, COLOR_MAGENTA, back) == ERR) return 1;
  if (init_pair(N_CYAN, COLOR_CYAN, back) == ERR) return 1;
  if (init_pair(N_WHITE, COLOR_WHITE, back) == ERR) return 1;

  int usleep(__useconds_t usec);  // avoid warnings
  int ch = 0;
  while (ch != 'q') {
    init_game();
    bool ok = true;
    while (!check_game()) {
      //getch();
      usleep(500000);
      memcpy(maze_copy, maze, sizeof maze);
      nodelay(stdscr, false);   // may request input then
      move_game(trans(maze_copy));
      nodelay(stdscr, true);
      while (getch() != ERR)
        ;
      if (loss_game()) {
        ok = false;
        break;
      }
    }
    finish_game(ok);
    nodelay(stdscr, false);
    for (ch = getch(); ch != 'c' && ch != 'q' ; ch = getch())
      ;
  }

  endwin();
  return 0;
}
