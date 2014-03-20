// Author: Guinao

#include <vector>
#include <map>
#include <algorithm>
#include "strategy.h"
using namespace std;

#define UNKNOW -1
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

int next_move_alpha(int maze[4][4])
{
  //printf("next_move_alpha\n");
  //for(int i=0; i<4; ++i){
  //	for(int j=0; j<4; ++j){
  //	 printf("%d ", maze[i][j]);
  //	}
  //	printf("\n");
  //}
  //printf("\n");
  int next[4][4];
  int goal = 0;
  int value;

  // move up
  value = 0;
  for(int i=0; i<4; ++i){
    int j=0;
    for(int nj=0; nj<4; ++nj){
      while(j<4 && maze[j][i]==0)
        ++j;
      if(j >= 4){
        next[nj][i] = 0;
      }
      else{
        next[nj][i] = maze[j][i];
        ++j;
        while(j<4 && maze[j][i]==0)
          ++j;
        if(j<4 && next[nj][i]==maze[j][i]){
          ++j;
          value += next[nj][i];
          next[nj][i] *= 2;
        }
      }
    }
  }
  if (value > 0){
    value += next_move_alpha(next);
    goal = max(goal, value);
  }

  // move left
  value = 0;
  for(int i=0; i<4; ++i){
    int j=0;
    for(int nj=0; nj<4; ++nj){
      while(j<4 && maze[i][j]==0)
        ++j;
      if(j >= 4){
        next[i][nj] = 0;
      }
      else{
        next[i][nj] = maze[i][j];
        ++j;
        while(j<4 && maze[i][j]==0)
          ++j;
        if(j<4 && next[i][nj]==maze[i][j]){
          ++j;
          value += next[i][nj];
          next[i][nj] *= 2;
        }
      }
    }
  }
  if (value>0){
    value += next_move_alpha(next);
    goal = max(goal, value);
  }

  // move right
  value = 0;
  for(int i=0; i<4; ++i){
    int j=3;
    for(int nj=3; nj>=0; --nj){
      while(j>=0 && maze[i][j]==0)
        --j;
      if(j < 0){
        next[i][nj] = 0;
      }
      else{
        next[i][nj] = maze[i][j];
        --j;
        while(j>=0 && maze[i][j]==0)
          --j;
        if(j>=0 && next[i][nj]==maze[i][j]){
          --j;
          value += next[i][nj];
          next[i][nj] *= 2;
        }
      }
    }
  }
  if (value>0){
    value += next_move_alpha(next);
    goal = max(goal, value);
  }

  // do not use move down

  return goal;
}

int next_move(int maze[4][4])
{
  int next[4][4];
  bool legal;
  int goal = 0;
  int value;
  int ret = DOWN;

  // move right
  value = 0;
  for(int i=0; i<4; ++i){
    int j=3;
    for(int nj=3; nj>=0; --nj){
      while(j>=0 && maze[i][j]==0)
        --j;
      if(j < 0){
        next[i][nj] = 0;
      }
      else{
        next[i][nj] = maze[i][j];
        --j;
        while(j>=0 && maze[i][j]==0)
          --j;
        if(j>=0 && next[i][nj]==maze[i][j]){
          --j;
          value += next[i][nj];
          next[i][nj] *= 2;
        }
      }
    }
  }
  legal = false;
  for(int i=0; i<4 && !legal; ++i){
    for(int j=0; j<4 && !legal; ++j){
      if(maze[i][j] != next[i][j])
        legal = true;
    }
  }
  if (legal){
    if(value > 0)
      value += next_move_alpha(next);
    if(value >= goal){
      goal = value;
      ret = RIGHT;
    }
  }

  // move left
  value = 0;
  for(int i=0; i<4; ++i){
    int j=0;
    for(int nj=0; nj<4; ++nj){
      while(j<4 && maze[i][j]==0)
        ++j;
      if(j >= 4){
        next[i][nj] = 0;
      }
      else{
        next[i][nj] = maze[i][j];
        ++j;
        while(j<4 && maze[i][j]==0)
          ++j;
        if(j<4 && next[i][nj]==maze[i][j]){
          ++j;
          value += next[i][nj];
          next[i][nj] *= 2;
        }
      }
    }
  }
  legal = false;
  for(int i=0; i<4 && !legal; ++i){
    for(int j=0; j<4 && !legal; ++j){
      if(maze[i][j] != next[i][j])
        legal = true;
    }
  }
  if (legal){
    if(value > 0)
      value += next_move_alpha(next);
    if(value >= goal){
      goal = value;
      ret = LEFT;
    }
  }

  // move up
  value = 0;
  for(int i=0; i<4; ++i){
    int j=0;
    for(int nj=0; nj<4; ++nj){
      while(j<4 && maze[j][i]==0)
        ++j;
      if(j >= 4){
        next[nj][i] = 0;
      }
      else{
        next[nj][i] = maze[j][i];
        ++j;
        while(j<4 && maze[j][i]==0)
          ++j;
        if(j<4 && next[nj][i]==maze[j][i]){
          ++j;
          value += next[nj][i];
          next[nj][i] *= 2;
        }
      }
    }
  }
  legal = false;
  for(int i=0; i<4 && !legal; ++i){
    for(int j=0; j<4 && !legal; ++j){
      if(maze[i][j] != next[i][j])
        legal = true;
    }
  }
  if (legal){
    if(value > 0)
      value += next_move_alpha(next);
    if(value >= goal){
      goal = value;
      ret = UP;
    }
  }

  // do not use move down

  return ret;
}
