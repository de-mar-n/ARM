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
#include "ED25519/ed25519.h"
#include "mbedtls/sha256.h"

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


/*RSApubKey_stt generate_public_key()
{
  uint8_t public_key [32]; // Buffer for the public key
  uint8_t signature[64];
  uint8_t Secret_key [64] =
    {0x4c, 0xcd, 0x08, 0x9b, 0x28, 0xff, 0x96, 0xda, 0x9d, 0xb6, 0xc3, 0x46,
0xec, 0x11, 0x4e, 0x0f,0x5b, 0x8a, 0x31, 0x9f, 0x35, 0xab, 0xa6, 0x24, 0xda,
0x8c, 0xf6, 0xed, 0x4f, 0xb8, 0xa6, 0xfb,0x3d, 0x40, 0x17, 0xc3, 0xe8, 0x43,
0x89, 0x5a, 0x92, 0xb7, 0x0a, 0xa7, 0x4d, 0x1b, 0x7e, 0xbc,0x9c, 0x98, 0x2c,
0xcf, 0x2e, 0xc4, 0x96, 0x8c, 0xc0, 0xcd, 0x55, 0xf1, 0x2a, 0xf4, 0x66,
0x0c};
const uint8_t ed25519_m[1] ={0x72};
{
 int32_t status = ED25519_SUCCESS;
 // generate public key from the secret key
 status = ED25519keyGen(Secret_key, public_key);
 if (status == ED25519_SUCCESS)
 {
   RSApubKey_stt pub_key_struct;
   pub_key_struct.
 }
 return NULL;
}
*/
RSApubKey_stt generate_public_key(){
  uint8_t public_exponent [3]= {0x01, 0x00, 0x01};
  uint8_t modulus [2048/8]= {42};

  RSApubKey_stt pubKey;
  // Set values of public key
  pubKey.mExponentSize = sizeof (public_exponent);
  pubKey.pmExponent = public_exponent;
  pubKey.mModulusSize = sizeof (modulus);
  pubKey.pmModulus = modulus;
  return pubKey;
}


RNGstate_stt generate_RNGstate()
{
  RNGstate_stt RNGstate;
  RNGinitInput_stt RNGinit_st;
  uint8_t entropy_data[32]={0x9d,0x20,0x1a,0x18,0x9b,0x6d,0x1a,0xa7,0x0e,0x79,0x57,0x6f,0x36,0xb6,0xaa,0x88,0x55,0xfd,0x4a,0x7f,0x97,0xe9,0x71,0x69,0xb6,0x60,
0x88,0x78,0xe1,0x9c,0x8b,0xa5};
 uint8_t nonce[4] = {0,1,2,3};
 uint8_t randombytes [16];

 RNGinit_st.pmEntropyData = entropy_data;
 RNGinit_st.mEntropyDataSize = sizeof (entropy_data);
 RNGinit_st.pmNonce = nonce;
 RNGinit_st.mNonceSize = sizeof (nonce);

 RNGinit_st.mPersDataSize = 0;
 RNGinit_st.pmPersData = NULL;

 /* Init the random engine */
 if ( RNGinit(&RNGinit_st, &RNGstate) != 0)
  myprintint(3, &huart1);
  /* Generate */
  uint8_t retval = RNGgenBytes(&RNGstate, NULL, randombytes,sizeof(randombytes));
  if (retval != 0)
    myprintint(4, &huart1);
  return RNGstate;
}


/**
  * @brief  RSA Signature Generation with PKCS#1v1.5
  * @param  P_pPrivKey The RSA private key structure, already initialized
  * @param  P_pInputMessage Input Message to be signed
  * @param  P_MessageSize Size of input message
  * @param  P_pOutput Pointer to output buffer
  * @retval error status: can be RSA_SUCCESS if success or one of
  * RSA_ERR_BAD_PARAMETER, RSA_ERR_UNSUPPORTED_HASH, RSA_ERR_BAD_KEY, ERR_MEMORY_FAIL
  * RSA_ERR_MODULUS_TOO_SHORT
*/


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

  while (1)
  {
    /* USER CODE END WHILE */
    // Set values of private key
  /*  privKey.mExponentSize = sizeof (private_exponent);
    privKey.pmExponent = private_exponent;
    privKey.mModulusSize = sizeof (modulus);
    privKey.pmModulus = modulus;
*/
    // generate the signature, knowing that the hash has been generated by SHA256
    /*int32_t retval;
    // ------------------ RNG --------------------
    RNGstate_stt RNGstate;
    RNGinitInput_stt RNGinit_st;
    uint8_t entropy_data[32]={0x9d,0x20,0x1a,0x18,0x9b,0x6d,0x1a,0xa7,0x0e,0x79,0x57,0x6f,0x36,0xb6,0xaa,0x88,0x55,0xfd,0x4a,0x7f,0x97,0xe9,0x71,0x69,0xb6,0x60,
  0x88,0x78,0xe1,0x9c,0x8b,0xa5};
   uint8_t nonce[4] = {0,1,2,3};
   uint8_t randombytes [16];

   RNGinit_st.pmEntropyData = entropy_data;
   RNGinit_st.mEntropyDataSize = sizeof (entropy_data);
   RNGinit_st.pmNonce = nonce;
   RNGinit_st.mNonceSize = sizeof (nonce);

   RNGinit_st.mPersDataSize = 0;
   RNGinit_st.pmPersData = NULL;


   if ( RNGinit(&RNGinit_st, &RNGstate) != 0)
    myprintint(3, &huart1);

    retval = RNGgenBytes(&RNGstate, NULL, randombytes,sizeof(randombytes));
    if (retval != 0)
      myprintint(4, &huart1);
  // ------------------end RNG --------------------

  // ----------------- PubKey ------------------
  uint8_t exponent [3]= {0x01, 0x00, 0x01};
  uint8_t modulus [2048/8]= {42};

  RSApubKey_stt pubKey;
  // Set values of public key
  pubKey.mExponentSize = sizeof (public_exponent);
  pubKey.pmExponent = public_exponent;
  pubKey.mModulusSize = sizeof (modulus);
  pubKey.pmModulus = modulus;
  // ----------------- end PubKey ------------------


    uint8_t output[2048];
    const uint8_t in = 123;

    RSAinOut_stt input;
    input.pmInput = &in;
    input.mInputSize = 5;
    input.pmOutput = output;
*/
/*
    membuf_stt membuf;
    uint8_t buff_membuf[2048];
    membuf.pmBuf = buff_membuf;
    membuf.mSize = 2048;
    membuf.mUsed = 0;

    //myprintint(8, &huart1);
    //retval = RSA_PKCS1v15_Encrypt(&pubKey, &input, &RNGstate, &membuf);
     RSAprivKey_stt privKey;
    uint8_t digest[CRL_SHA256_SIZE]= "nb";
    uint8_t signature[2048/8];

      // Set values of private key
      privKey.mExponentSize = sizeof(exponent);
      privKey.pmExponent = exponent;
      privKey.mModulusSize = sizeof(modulus);
      privKey.pmModulus = modulus;

    uint32_t status =  RSA_PKCS1v15_Sign(&privKey, digest, E_SHA256, signature, &membuf);
    myprintint(status, &huart1);
//------------- COMMUNICATION UART ---------------------
*/
int i;
 unsigned char x[32];
 unsigned char y[256];
 size_t y_len = sizeof(y);
 memset(x, 0, sizeof(x));
 memset(y, 0, sizeof(y));

 // Here you should fill Y with the data you want to hash!
 // Example: y[0]=1;

 // y_len should be set to the length of the data you put in Y.
 // Example: y_len=1;

 mbedtls_sha256(y, y_len, x, 0);

 // X now contains SHA256(y)
 // Print it
 for(i = 0; i < 32; i++)
     myprintf(x, &huart1);
  }


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
