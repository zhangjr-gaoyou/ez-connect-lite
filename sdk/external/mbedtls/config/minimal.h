#ifndef MINIMAL_CONFIG_H
#define MINIMAL_CONFIG_H

#include "wmsdk_platform_config.h"

/* Enable the debug functions. */
#ifdef CONFIG_WM_MBEDTLS_DEBUG
  #define MBEDTLS_DEBUG_C
#endif /* CONFIG_WM_MBEDTLS_DEBUG */

/*----------------------------------------------------------------------
 * Enable the generic cipher layer
 */
#define MBEDTLS_CIPHER_C
/* Enable the generic SSL/TLS code */
#define MBEDTLS_SSL_TLS_C
/* Enable the generic message digest layer */
#define MBEDTLS_MD_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the SSL/TLS client and server code
 */
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 */
/* Enable sending of all alert messages */
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES

/* Maxium fragment length in bytes, determines the size of
 * each of the two internal I/O buffers */
#define MBEDTLS_SSL_MAX_CONTENT_LEN			(1024 * 4)

/* Remove RC4 ciphersuites by default in SSL / TLS. */
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable specific cipher modes
 */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_MODE_CFB
#define MBEDTLS_CIPHER_MODE_CTR
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable AES block cipher
 */
#define MBEDTLS_AES_C

/* Enable the CTR_DRBG AES-256-based random generator. */
#define MBEDTLS_CTR_DRBG_C

/* Enable the Counter with CBC-MAC (CCM) mode for
 * 128-bit block cipher. */
#define MBEDTLS_CCM_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable padding modes in the cipher layer.
 */
#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the particular set of ciphersuite modes in SSL / TLS.
 */
#define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED

/* Enable the RSA public-key cryptosystem. */
#define MBEDTLS_RSA_C
/* Enable the Diffie-Hellman-Merkle module. */
#define MBEDTLS_DHM_C

#define MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable error code to error string conversion
 */
#define MBEDTLS_ERROR_C
#define MBEDTLS_ERROR_STRERROR_DUMMY
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the test certificates.
 */
#define MBEDTLS_CERTS_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable support for particular protocol
 */
#define MBEDTLS_SSL_PROTO_TLS1
#define MBEDTLS_SSL_PROTO_TLS1_1
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the multi-precision integer library
 */
#define MBEDTLS_BIGNUM_C

/* Enable the prime-number generation code. */
#define MBEDTLS_GENPRIME
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable X.509 core for using certificates.
 */
#define MBEDTLS_X509_USE_C

/* Enable X.509 certificate parsing */
#define MBEDTLS_X509_CRT_PARSE_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the Base64 module.
 */
#define MBEDTLS_BASE64_C

/* Enable PEM decoding / parsing. */
#define MBEDTLS_PEM_PARSE_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the generic public (asymetric) key layer.
 */
#define MBEDTLS_PK_C

/* Enable the generic public (asymetric) key parser. */
#define MBEDTLS_PK_PARSE_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the particular hash algorithm
 */
#define MBEDTLS_MD5_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 */
/* Enable support for PKCS#1 v1.5 encoding. */
#define MBEDTLS_PKCS1_V15
/* Enable support for PKCS#1 v2.1 encoding. */
#define MBEDTLS_PKCS1_V21

/* Enable support for Encrypt-then-MAC, RFC 7366. */
#define MBEDTLS_SSL_ENCRYPT_THEN_MAC

/* Enable support for FALLBACK_SCSV */
#define MBEDTLS_SSL_FALLBACK_SCSV

/* Enable 1/n-1 record splitting for CBC mode in SSLv3
 * and TLS 1.0. */
#define MBEDTLS_SSL_CBC_RECORD_SPLITTING

/* Disable support for TLS renegotiation. */
#define MBEDTLS_SSL_RENEGOTIATION

/* Enable support for RFC 6066 max_fragment_length extension
 * in SSL. */
#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH

/* Enable the generic ASN1 parser and writer */
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C

/* Enable the OID database. */
#define MBEDTLS_OID_C

/* Enable PKCS#5 functions. */
#define MBEDTLS_PKCS5_C

/* Enable the HMAC_DRBG random generator. */
#define MBEDTLS_HMAC_DRBG_C

/* Enable verification of the keyUsage extension (CA and
 * leaf certificates). */
#define MBEDTLS_X509_CHECK_KEY_USAGE
/* Enable support for RFC 6066 server name
 * indication (SNI) in SSL. */
#define MBEDTLS_SSL_SERVER_NAME_INDICATION
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Enable the generic public (asymetric) key writer.
 */
#define MBEDTLS_PK_WRITE_C

#include <mbedtls/check_config.h>
/*----------------------------------------------------------------------*/
#endif /* MINIMAL_CONFIG_H */
