
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


//#define KEY_SIZE 2048
#define KEY_SIZE 128 // for testing
#define EXPONENT 65537


int generate_RSA_key( void )
{
    int ret = 2;
    mbedtls_rsa_context rsa;
    mbedtls_hmac_drbg_context rng_ctx;
    const mbedtls_md_info_t *md_info;
    const unsigned char *seed = "seedishere"; //seed for random number generator

    mbedtls_hmac_drbg_init( &rng_ctx );
    if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
            return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    HAL_Delay(1000);
    mbedtls_md_init(&rng_ctx.md_ctx);
    ret = mbedtls_hmac_drbg_seed_buf( &rng_ctx, md_info, seed, 10 );
    HAL_Delay(1000);

    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0);
    HAL_Delay(20000);

    if( ( ret = mbedtls_rsa_gen_key( &rsa, mbedtls_hmac_drbg_random, &rng_ctx, KEY_SIZE,
                             EXPONENT ) ) != 0 )
    {
      HAL_Delay(20000);
        ret = 5678;
        return ret;
        goto exit;
    }
    ret = 123;
    return ret;

exit:

    return( ret );
}
