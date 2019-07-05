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

uint8_t hex_to_int(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  else
    return 255;
}

void sha256_to_binary(char* sha256_str, uint8_t *binary)
{
  for (int i = 0; i < 32; i++)
  {
    binary[i] = hex_to_int(sha256_str[2*i]) * 16
      + hex_to_int(sha256_str[2*i + 1]);
  }
}
