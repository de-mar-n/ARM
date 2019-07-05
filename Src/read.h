#pragma once
#include "main.h"
#include <stdlib.h>
#include <stdbool.h>

void read_until(char c, char* buffer, size_t size, UART_HandleTypeDef *huart);
