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


int generate_RSA_key( void );
