#include "read.h"
#include "myprintf.h"

extern bool received;

void read_until(char c, char* buffer, size_t size, UART_HandleTypeDef *huart)
{
  char tmp = 0;
  size_t i = 0;
  HAL_UART_Receive_IT(huart, &tmp, 1);
  while (!received);
  received = false;
  while (tmp != c && i < size - 1)
  {
    buffer[i] = tmp;
    i++;
    HAL_UART_Receive_IT(huart, &tmp, 1);
    while (!received);
    received = false;
  }
  buffer[i] = 0;
}
