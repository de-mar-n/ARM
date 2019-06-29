#include "myprintf.h"
void myprintf(char *str, UART_HandleTypeDef* huart, ...)
{
   va_list argptr;
   char buffer[MY_PRINTF_BUFFER_SIZE];
   sprintf(buffer, str, argptr);
   HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), 1000);
}

void myprintint(int n, UART_HandleTypeDef* huart)
{
   char buffer[MY_PRINTF_BUFFER_SIZE];
   sprintf(buffer, "%d", n);
   HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), 1000);
}

char uint8tohex(uint8_t u)
{
  if (u > 9)
    return 'a' + u - 10;
  else
    return '0' + u;
}

void myprintuint8ashex(uint8_t u, UART_HandleTypeDef* huart)
{
   char c[3];
   c[0] = uint8tohex(u / 16);
   c[1] = uint8tohex(u % 16);
   c[2] = 0;
   HAL_UART_Transmit(huart, (uint8_t*) c, 2, 1000);
}
