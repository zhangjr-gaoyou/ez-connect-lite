#ifndef WM_MBEDTLS_ENTROPY_H
#define WM_MBEDTLS_ENTROPY_H

#include <mbedtls/ctr_drbg.h>

int wm_mbedtls_entropy_ctr_drbg_setup();

mbedtls_ctr_drbg_context *wm_mbedtls_get_ctr_drbg_ctx();

#endif /* WM_MBEDTLS_ENTROPY_H */
