#pragma once

#include <stdio.h>
#include <string.h>

#include "mbedtls.h"
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/bignum.h"
#include "mbedtls/x509.h"
#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "myprintf.h"

#include "mbedtls/x509_crt.h"


union int_or_RSA
{
  int int_error;
  mbedtls_pk_context pk_key;
};

union int_or_RSA generate_RSA_key( void );
int export_key_on_UART(/*UART_HandleTypeDef *uart*/);
