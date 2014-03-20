#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#ifdef __cplusplus
extern "C"
#endif
int next_move(int maze[4][4]);

#ifdef __cplusplus
extern "C"
#endif
int roundrobin(int maze[4][4]);

#ifdef __cplusplus
extern "C"
#endif
int roundrobin2(int maze[4][4]);

#ifdef __cplusplus
extern "C"
#endif
int user_input(int maze[4][4]);

#ifdef __cplusplus
extern "C"
#endif
int move_action(int maze[4][4], int action);

#endif
