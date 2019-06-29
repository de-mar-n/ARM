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
#include "Common/err_codes.h"
#include "HASH/hash.h"
#include "HASH/Common/hash_common.h"
#include "HASH/SHA256/sha256.h"
#include "RNG/rng.h"
#include "RSA/rsa.h"


//#include "RSA/PKCS#1v15/rsa_pkcs1v15.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

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
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int update_binary(void)
{
  unsigned char out[32];
  unsigned char d = 'j';
  HAL_UART_Transmit(&huart1, &d, 1, 200);
  memset(out, 36, sizeof(out));
  HAL_UART_Transmit(&huart1, out, 32, 200);
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
//  unsigned char c;
/*  HAL_UART_Receive (&huart1, &c, 1, 5000);
  if (c == 'u') {
    c = 'a';
    HAL_UART_Transmit(&huart1, &c, 1, 200);
    int ret = 0;
    do {
      ret = update_binary();
    } while (ret == -1);
  }*/
  char*c="s";
  myprintf(c, &huart1);

  unsigned char hello[] = "hello!";
  //unsigned char out[32];
  uint8_t tmp[32];
  int32_t sz = 32;

  SHA256ctx_stt ctx;
//  c = 'j';
  //HAL_UART_Transmit(&huart1, &c, 1, 200);
  ctx.mFlags = E_HASH_DEFAULT;
  ctx.mTagSize = 32;
  int ret = SHA256_Init(&ctx);
  /*if (ret == HASH_ERR_BAD_PARAMETER)
    HAL_UART_Transmit(&huart1, &c, 1, 300);
  c = 'k';
  HAL_UART_Transmit(&huart1, &c, 1, 200);
  */
  SHA256_Append(&ctx, hello, sizeof(hello));
  /*c = 'z';
  HAL_UART_Transmit(&huart1, &c, 1, 200);*/
  SHA256_Finish(&ctx, tmp, &sz);
//  c = 's';
  /*HAL_UART_Transmit(&huart1, &c, 1, 200);
  for (int i = 0; i < 32; ++i)
    HAL_UART_Transmit(&huart1, &out[i], 1, 200);
  HAL_UART_Receive_IT(&huart1, &c, 1);*/



  uint8_t modulus [2048/8];//= {...};
  uint8_t public_exponent [3]= {0x01, 0x00, 0x01};
  uint8_t digest [CRL_SHA256_SIZE];//= {...};
  //uint8_t signature [2048/8]; //FIXME
  uint8_t *signature = tmp;
  uint8_t private_exponent [2048/8];//= {...};
  int32_t retval;
  RSAprivKey_stt privKey;
  const RSApubKey_stt pubKey;


  while (1)
  {
    /* USER CODE END WHILE */
    // Set values of private key
    privKey.mExponentSize = sizeof (private_exponent);
    privKey.pmExponent = private_exponent;
    privKey.mModulusSize = sizeof (modulus);
    privKey.pmModulus = modulus;

    // generate the signature, knowing that the hash has been generated by SHA256
    membuf_stt membuf0[2048];
    membuf_stt membuf[2048];
    RNGstate_stt *P_pRandomState;
    RSAinOut_stt *input;
    int32_t retval = RSA_PKCS1v15_Encrypt(&pubKey, input, P_pRandomState, membuf);

  /*  retval = RSA_PKCS1v15_Sign(&privKey, digest, E_SHA256, signature, membuf0);
    if (retval != RSA_SUCCESS)
    {return(ERROR); }
    // Set values of public key
    pubKey.mExponentSize = sizeof (public_exponent);
    pubKey.pmExponent = public_exponent;
    pubKey.mModulusSize = sizeof (modulus);
    pubKey.pmModulus = modulus;
    // verify the signature, knowing that the hash has been generated by SHA-256

    retval = RSA_PKCS1v15_Verify(&pubKey, digest, E_SHA256, signature, membuf);

    if (retval != SIGNATURE_VALID )
    {
      myprintint(4567, &huart1);
      //return (ERROR);
    }
    else
    {
      myprintint(789, &huart1);
      //return (OK);
    }*/
  }

    /*if (received)
    {
      received = false;
      HAL_UART_Transmit (&huart1, &c, 1, 200);
      HAL_UART_Receive_IT (&huart1, &c, 1);


    }*/
    /* USER CODE BEGIN 3 */
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
  huart1.Init.BaudRate = 115200;
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
