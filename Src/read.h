#pragma once
#include "main.h"
#include <stdlib.h>
#include <stdbool.h>

void read_until(char c, char* buffer, size_t size, UART_HandleTypeDef *huart);

void sha256_to_binary(char* sha256_str, uint8_t *binary);
