
/*
 *  Example RSA key generation program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "my_rsa.h"

// For RSA generation
//#define KEY_SIZE 2048
#define KEY_SIZE 128 // for testing
#define EXPONENT 65537

// For exporting on UART
#define KEY_FORMAT MBEDTLS_PK_RSA
#define BUFFER_SIZE 16000


//FIXME
//  Sign a hash (sha-256) given by UART with private key
int sign_hash(char *hash)
{
  hash = hash;
  return 0;
}
// ------------------- End -------------------

// ------------------- Export on UART -------------------
/*unsigned char *get_public_key_hash(mbedtls_pk_context *key)
{
  int ret;
  unsigned char output_buf[BUFFER_SIZE];
  //unsigned char *c = output_buf;
  size_t len = 0;

  memset(output_buf, 0, BUFFER_SIZE);
  if( ( ret = mbedtls_pk_write_key_pem( key, output_buf, BUFFER_SIZE ) ) != 0 )
    return NULL;
  len = strlen( (char *) output_buf );

  return output_buf;
}*/

int export_key_on_UART(UART_HandleTypeDef *uart)
{
  mbedtls_pk_context pk_key = (generate_RSA_key()).pk_key;
  // Check the type and init the pk context
  /*if( mbedtls_pk_get_type( key ) == MBEDTLS_PK_RSA )
  {
     mbedtls_rsa_context *rsa = mbedtls_pk_rsa( *key );
     // Exports the core parameters of an RSA key
     if( ( ret = mbedtls_rsa_export    ( rsa, &rsa->N, &rsa->P, &rsa->Q, &rsa->D, &rsa->E ) ) != 0 ||
               ( ret = mbedtls_rsa_export_crt( rsa, &rsa->DP, &rsa->DQ, &rsa->QP ) )      != 0 )
         return ret;

  }
  else
    return 3; // wrong key type FIXME
*/

//  output = get_public_key_hash( &pk_key );

int ret;
int len;
unsigned char output_buf[BUFFER_SIZE];
//unsigned char *c = output_buf;
//HAL_Delay(1000);
memset(output_buf, 0, BUFFER_SIZE);
HAL_Delay(1000);
if( ( ret = mbedtls_pk_write_pubkey_pem( &pk_key, output_buf, BUFFER_SIZE ) ) != 0 )
  return "pas bon";
HAL_Delay(1000);
myprintf(output_buf, uart);
HAL_Delay(1000);
len = strlen( (char *) output_buf );
  return len;
}
// ------------------- End -------------------

// ------------------- Generate RSA keys -------------------
union int_or_RSA generate_RSA_key( void )
{
    union int_or_RSA result;
    int ret;
  //  mbedtls_rsa_context rsa;
    mbedtls_hmac_drbg_context rng_ctx;
    const mbedtls_md_info_t *md_info;
    const unsigned char *seed = "seedishere"; //seed for random number generator

    // Init random genrator
    mbedtls_hmac_drbg_init( &rng_ctx );
    if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
    {
      result.int_error = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
      return result;
    }

    // Seeding
    HAL_Delay(1000);
    mbedtls_md_init(&rng_ctx.md_ctx);
    ret = mbedtls_hmac_drbg_seed_buf( &rng_ctx, md_info, seed, 10 );
    HAL_Delay(1000);

    // Generate the key
  //  mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0);
    HAL_Delay(20000);

//---------------------------------------

    mbedtls_pk_context key;

    // Initialize a PK context
    mbedtls_pk_init( &key );
    if( ( ret = mbedtls_pk_setup( &key,
                mbedtls_pk_info_from_type( (mbedtls_pk_type_t) KEY_FORMAT ) ) ) != 0 )
    {
      // Cannot init pk
      result.int_error = ret;
      return result;
    }
// ---------------------------------
    HAL_Delay(1000);
    // Generate the key
    if( ( ret = mbedtls_rsa_gen_key( mbedtls_pk_rsa( key ), mbedtls_hmac_drbg_random, &rng_ctx, KEY_SIZE,
                             EXPONENT ) ) != 0 )
    {
      HAL_Delay(20000);
      result.int_error = ret;
      return result;
    }

    result.pk_key = key;
    return result;
}
