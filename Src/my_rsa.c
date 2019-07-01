
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

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "mbedtls.h"
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/bignum.h"
#include "mbedtls/x509.h"
#include "mbedtls/rsa.h"



#define KEY_SIZE 2048
#define EXPONENT 65537


int generate_RSA_key( void )
{
    int ret = 2;
    mbedtls_rsa_context rsa;
    mbedtls_hmac_drbg_context rng_ctx;
    const mbedtls_md_info_t *md_info;
    const unsigned char *seed = "seedishere"; //seed for random number generator

    mbedtls_hmac_drbg_init( &rng_ctx );
    /*if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );*/

    if( ( md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 ) ) == NULL )
            return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    HAL_Delay(1000);
    ret = mbedtls_hmac_drbg_seed_buf( &rng_ctx, md_info, seed, 10 );
    HAL_Delay(1000);

return ret;
    //mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0);
    if( ( ret = mbedtls_rsa_gen_key( &rsa, mbedtls_hmac_drbg_random, &rng_ctx, KEY_SIZE,
                             EXPONENT ) ) != 0 )
    {
      ret = 1;
        goto exit;
    }

  /*  if( ( ret = mbedtls_mpi_write_file( "N = " , &rsa.N , 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "E = " , &rsa.E , 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "D = " , &rsa.D , 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "P = " , &rsa.P , 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "Q = " , &rsa.Q , 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "DP = ", &rsa.DP, 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "DQ = ", &rsa.DQ, 16, fpriv ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "QP = ", &rsa.QP, 16, fpriv ) ) != 0 )
    {
      ret = 9;
        goto exit;
    }*/
ret = 8;
exit:

    return( ret );
}
