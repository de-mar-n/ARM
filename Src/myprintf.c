#include "myprintf.h"
void myprintf(char *str, UART_HandleTypeDef* huart, ...)
{
   va_list argptr;
   char buffer[MY_PRINTF_BUFFER_SIZE];
   sprintf(buffer, str, argptr);
   HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), 1000);
}
