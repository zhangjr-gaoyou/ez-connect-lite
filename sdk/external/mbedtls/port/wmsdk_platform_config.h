#ifndef WMSDK_PLATFORM_CONFIG_H
#define WMSDK_PLATFORM_CONFIG_H

#include <wmstdio.h>

/*----------------------------------------------------------------------
 * Enable the platform abstraction layer
 */
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_PRINTF_ALT
#define MBEDTLS_PLATFORM_STD_PRINTF        wmprintf
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the platform-specific entropy code
 */
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#define MBEDTLS_NO_PLATFORM_ENTROPY
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the memory allocation layer.
 */
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_PLATFORM_STD_CALLOC		NULL
#define MBEDTLS_PLATFORM_STD_FREE		NULL

/*----------------------------------------------------------------------
 * Enable the semi-portable timing interface.
 */
#define MBEDTLS_TIMING_C
#define MBEDTLS_TIMING_ALT
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the threading abstraction layer.
 */
#define MBEDTLS_THREADING_C
#define MBEDTLS_THREADING_ALT
/*----------------------------------------------------------------------*/

/* The compiler has support for asm(). */
#define MBEDTLS_HAVE_ASM

/* System has time.h and time(). */
#define MBEDTLS_HAVE_TIME
/* System has time.h and time(), gmtime() and the clock is correct. */
#define MBEDTLS_HAVE_TIME_DATE

/*----------------------------------------------------------------------*/
#endif /* WMSDK_PLATFORM_CONFIG_H */
