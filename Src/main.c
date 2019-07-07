/* USER CODE BEGIN Header */ /** ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "myprintf.h"
#include "mbedtls/md.h"
#include "mbedtls/debug.h"
#include "mbedtls/sha256.h"
#include "Common/err_codes.h"
#include "HASH/hash.h"
#include "HASH/Common/hash_common.h"
#include "HASH/SHA256/sha256.h"
#include "sha256.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static char *welcome = "Welcome to a bootloader better than GRUB2!";
static char *update_mess = "Updating, please wait";
static char *erase_err = "Error during erase of the flash\n";
static char *hash_err = "Error hash not equivalent\n";
static char *flash_err = "Error during the flash reprogramming\n";
static char *comma =",";

UART_HandleTypeDef huart1;
static bool received = false;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    received = true;
  }
}

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
int update_binary(void);
int erase_sector(void);
int read_and_flash(void);
int flash_chunk(uint8_t *new_firm, uint32_t cnt, uint16_t size);
void jump_to_app(void);
void check_flash(uint8_t hash_firm[32], uint32_t size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void check_flash(uint8_t hash_firm[32], uint32_t size)
{
  SHA256_CTX ctx;
  uint8_t comp_hash[32] = {0};
  uint8_t new_firm[256] = {0};
  uint32_t cnt = 0;

  sha256_init(&ctx);
  for (int i = 0; i < (size / 256); ++i)
  {
    memcpy(new_firm, 0x8020000 + i * 256, 256);
    sha256_update(&ctx, new_firm, 256);
    memset(new_firm, 0, sizeof(new_firm));
    ++cnt;
  }
  if ((size % 256) != 0)
  {
    memcpy(new_firm, 0x8020000 + cnt * 256, size % 256);
    sha256_update(&ctx, new_firm, size % 256);
    ++cnt;
  }
  sha256_final(&ctx, comp_hash);
  HAL_UART_Transmit(&huart1, (uint8_t*)"check ", strlen("check "), 200);
  HAL_UART_Transmit(&huart1, (uint8_t *)comp_hash, strlen(comp_hash), 200);
}

void jump_to_app(void)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)"jump ", 5, 200);
  uint32_t addr = 0x08020000;

  HAL_DeInit();
 // __disable_irq();
  __set_MSP(*(__IO uint32_t *)addr);
  void (**user_entry)(void) = (void*)(addr + 4);
  (**user_entry)();

}

int flash_chunk(uint8_t *new_firm, uint32_t cnt, uint16_t size)
{
  uint32_t i = 0;
  HAL_StatusTypeDef ret = 0;

  while (i < size)
    {
      ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, 0x08020000 + 256 * cnt + i, new_firm[i]);
      if (ret != HAL_OK)
      {
        HAL_UART_Transmit(&huart1, (uint8_t*)flash_err, strlen(flash_err), 300);
        HAL_UART_Transmit(&huart1, (uint8_t *)"final", strlen("final"), 300);
        return -1;
      }
      ++i;
    }
  return 0;
}

int read_and_flash(void)
{
  uint32_t sz = 0;
  int8_t recv_size[10] = {0}; // 10 numbers + end
  uint8_t hash_firm[32] = {0}; //hash
  uint8_t comp_hash[32] = {0}; //computed hash
  uint8_t new_firm[256] = {0}; // blocks of 256 bytes
  uint32_t cnt = 0; // number of blocks of firm written already
  SHA256_CTX ctx;

  sha256_init(&ctx);
  HAL_UART_Transmit(&huart1, (uint8_t*)"Computing sha", strlen("Computing sha"), 200);
  HAL_UART_Transmit(&huart1, (uint8_t*)".", 1, 200);
  HAL_UART_Receive_IT(&huart1, (uint8_t*)recv_size, 10);
  while (!received);
  received = false;
  HAL_UART_Transmit(&huart1, (uint8_t*)recv_size, 10, 200);
  HAL_UART_Transmit(&huart1, (uint8_t*)".", 1, 200);
  HAL_UART_Receive_IT(&huart1, hash_firm, 32);
  while (!received);
  received = false;
  HAL_UART_Transmit(&huart1, (uint8_t*)hash_firm, 32, 200);
  HAL_UART_Transmit(&huart1, (uint8_t*)comma, 1, 200);
  sz = atoi((char*)recv_size);

  for (int i = 0; i < (sz / 256); ++i)
  {
    myprintint(i, &huart1);
    HAL_UART_Transmit(&huart1, (uint8_t*)".", 1, 200);
    HAL_UART_Receive_IT(&huart1, new_firm, 256);
    while (!received);
    received = false;
    sha256_update(&ctx, new_firm, 256);
    flash_chunk(new_firm, cnt, 256);
    ++cnt;
    memset(new_firm, 0, sizeof(new_firm));
  }
  if ((sz % 256) != 0)
  {
    HAL_UART_Transmit(&huart1, (uint8_t*)"else", strlen("else"), 200);
    HAL_UART_Transmit(&huart1, (uint8_t*)".", 1, 200);
    HAL_UART_Receive_IT(&huart1, new_firm, sz % 256);
    while (!received);
    received = false;
    HAL_UART_Transmit(&huart1, (uint8_t*)comma, 1, 500);
    sha256_update(&ctx, new_firm, sz % 256);
    flash_chunk(new_firm, cnt, sz % 256);
    ++cnt;
  }

  sha256_final(&ctx, comp_hash);
  HAL_UART_Transmit(&huart1, (uint8_t*)comp_hash, 32, 200);
  for (int i = 0; i < 32; ++i)
  {
    if (comp_hash[i] != hash_firm[i]) {
      HAL_UART_Transmit(&huart1, (uint8_t*)hash_err, strlen(hash_err), 500);
      return -1;
    }
  }
  check_flash(hash_firm, sz);
  HAL_UART_Transmit(&huart1, (uint8_t *)"final", strlen("final"), 200);

  return 0;
}
int erase_sector(void)
{
  HAL_StatusTypeDef retval;
  uint32_t sect_err = 0;
  FLASH_EraseInitTypeDef erase;
  erase.TypeErase = FLASH_TYPEERASE_SECTORS;
  erase.Sector = FLASH_SECTOR_5;
  erase.NbSectors = 2;
  erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  retval = HAL_FLASHEx_Erase(&erase, &sect_err);
  if (retval != HAL_OK) {
    HAL_UART_Transmit(&huart1, (uint8_t*)erase_err, strlen(erase_err), 500);
    HAL_UART_Transmit(&huart1, (uint8_t*)sect_err, 8, 500);
    return -1;
  }

  return 0;
}

int update_binary(void)
{
  int ret;
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    |
                       FLASH_FLAG_WRPERR |
                       FLASH_FLAG_PGPERR | FLASH_FLAG_OPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
  HAL_UART_Transmit(&huart1, (uint8_t *)update_mess, strlen(update_mess), 200);
  ret = erase_sector();
  if (ret == -1)
    return -1;

  ret = read_and_flash();
  if (ret == -1)
    return -1;

  HAL_FLASH_Lock();
  return 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UART_Transmit(&huart1, (uint8_t *)welcome, strlen(welcome), 200);
  unsigned char c;
  HAL_UART_Receive (&huart1, &c, 1, 5000);
  if (c == 'u') {
    c = 'a';
    int ret = 0;
    do {
    //  HAL_UART_Receive_IT(&huart1, &c, 1);
      ret = update_binary();
    } while (ret == -1);
  }
  jump_to_app();
  HAL_UART_Receive_IT(&huart1, &c, 1);
  while (1)
  {
    /* USER CODE END WHILE */

    if (received)
    {
      received = false;
      HAL_UART_Transmit (&huart1, &c, 1, 200);
      HAL_UART_Receive_IT (&huart1, &c, 1);
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
