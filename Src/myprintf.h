#ifndef MYPRINT_F
#define MYPRINT_F
#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#define MY_PRINTF_BUFFER_SIZE 1024
void myprintf(char *str, UART_HandleTypeDef* huart, ...);
#endif
