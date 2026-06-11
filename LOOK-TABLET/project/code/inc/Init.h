#ifndef CODE_INC_INIT_H_
#define CODE_INC_INIT_H_

#pragma once

#define   PERIODMS     5         //采样周期（毫秒）  //迷宫法用5ms 祖传3ms就可以了?

extern pthread_mutex_t mutex;

void Peripheral_Init(void);
void sigint_handler(int signum);
void cleanup(void);
void Flag_clear(void);

#endif