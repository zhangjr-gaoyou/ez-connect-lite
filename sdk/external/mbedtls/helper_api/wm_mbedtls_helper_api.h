/*! \file wm_mbedtls_helper_api.h
 *  \brief An abstraction layer for mbed TLS stack
 */
/*
 * Copyright (C) 2017, Marvell International Ltd.
 * All Rights Reserved.
 */

#ifndef WM_MBEDTLS_HELPER_H
#define WM_MBEDTLS_HELPER_H

#include <mbedtls/ssl.h>
#include <mbedtls/pk.h>
#include <wm_mbedtls_debug.h>

/* NOTE:
 *
 * ca_chain object will be interpreted differently
 * in case of two endpoints viz. 'CLIENT' or 'SERVER'.
 *
 * For 'CLIENT', 'ca_chain' refers to 'Certifying Authority (CA)
 * certificate chain' present with CLIENT, which will be used to verify
 * SERVER i.e. for 'SERVER verification by CLIENT'
 *
 * For 'SERVER', 'ca_chain' refers to 'chain of client certificates'
 * present with SERVER, which will be used to verify CLIENT.
 * i.e. 'CLIENT verification by SERVER'
 */

typedef struct {
	mbedtls_x509_crt *ca_chain;
	mbedtls_x509_crt *own_cert;
	mbedtls_pk_context *own_key;
} wm_mbedtls_cert_t;

/**
 * Initialize MBEDTLS library pre-requisites as following:
 *
 * Initialize time subsystem including RTC.
 * Set memory callback functions for alloc, free
 * Set threading callback function for mutex free, lock, unlock
 * Setup global entropy, CTR_DRBG context.
 *
 * @return 0		Success
 * @return -1		Failed in setup for entropy, CTR_DRBG context.
 */
int wm_mbedtls_lib_init();

/**
 * Get wm_mbedtls library initialization status
 *
 * return true if initialized, false if not.
 */
bool is_wm_mbedtls_lib_init();

/**
 * Parse char buffer into MBEDTLS X.509 certificate container
 *
 * This function allocates memory for MBEDTLS X.509 certificate
 * container and parses input char buffer with its length into
 * MBEDTLS X.509 certificate container.
 *
 * After parsing is done, then char buffer can be freed if
 * dynamically allocated.
 *
 * Note: For PEM cert buffers, cert_buf must be null terminated
 * and its length must account for the last null character '\0'.
 * For DER cert buffers there is no such requirement.
 *
 * @param[in] cert_buf		A char cert buffer.
 * @param[in] cert_buf_len	Length of cert_buf
 *
 * @return MBEDTLS X.509 certificate container, NULL in case of parse failure.
 */
mbedtls_x509_crt *wm_mbedtls_parse_cert(const unsigned char *cert_buf,
		size_t cert_buf_len);

/**
 * Free MBEDTLS X.509 certificate container
 *
 * This function will free memory for following things:
 *
 * Allocations done by MBEDTLS itself internally.
 * X.509 certificate container allocated by WMSDK.
 *
 * @param[in] cert		Pointer to MBEDTLS X.509 certificate container
 */
void wm_mbedtls_free_cert(mbedtls_x509_crt *cert);

/**
 * Parse char buffer into MBEDTLS public key container
 *
 * This function allocates memory for MBEDTLS public key container
 * and parses input char buffer with its length into
 * MBEDTLS public key container.
 *
 * After parsing is done, then char buffer can be freed if
 * dynamically allocated.
 *
 * Note: For PEM key buffers, key_buf must be null terminated
 * and its length must account for the last null character '\0'.
 * For DER key buffers there is no such requirement.
 *
 * @param[in] key_buf		A char key buffer of X.509 key.
 * @param[in] key_len		Length of key_buf
 *
 * @param[in] pwd			Password string used to encrypt
 * X.509 key. If password string is not used then pass 'NULL' here.
 *
 * @param[in] pwd_len		Length of pwd
 *
 * @return MBEDTLS public key container, NULL in case of parse failure.
 */
mbedtls_pk_context *wm_mbedtls_parse_key(const unsigned char *key_buf,
		size_t key_len, const unsigned char *pwd, size_t pwd_len);

/**
 * Free MBEDTLS public key container
 *
 * This function will free memory for following things:
 *
 * Allocations done by MBEDTLS itself internally
 * Public key container allocated by WMSDK
 *
 * @param[in] key		Pointer to MBEDTLS public key container
 */
void wm_mbedtls_free_key(mbedtls_pk_context *key);

#ifdef MBEDTLS_DEBUG_C
/**
 * Set debug function callback and debug threshold in SSL configuration context
 *
 * Please note that, MBEDTLS_DEBUG_C must be defined to use this api.
 *
 * @param[in] conf		MBEDTLS SSL configuration context
 * @param[in] threshold	Debug threshold (Possible values: 0, 1, 2, 3, 4)
 * @param[in] f_dbg		Function pointer to set in SSL configuration
 * context. If 'NULL', then global dbg function will be set.
 */
void wm_mbedtls_set_debug_cb(mbedtls_ssl_config *conf,
		int threshold,
		void (*f_dbg)(void *, int, const char *, int, const char *));
#endif /* MBEDTLS_DEBUG_C */

/**
 * Create MBEDTLS SSL configuration context
 *
 * This function create SSL configuration context as per
 * input certificate structure, endpoint and authentication
 * mode.
 *
 * Memory allocated and default values are set in SSL configuration
 * context with input endpoint.
 *
 * Certificate structure is parsed and different entities in it such as
 * ca_chain, own_cert, own_key are set.
 *
 * CTR_DRBG context along with MBEDTLS DRBG function (to generate
 * random data) are registered in SSL configuration context.
 *
 * @param[in] cert			Certificate structure of ca_chain,
 * own_cert, own_key. This structure should be populated by user.
 * Appropriate buffers should be parsed into cert/key using apis
 * wm_mbedtls_parse_cert or wm_mbedtls_parse_key.
 *
 * @param[in] endpoint		Endpoint i.e. device is client or server.
 * There are 2 possible values: MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_IS_SERVER
 *
 * @param[in] authmode		Authentication mode to be imposed on peer-side.
 * There are 4 possible values:
 * MBEDTLS_SSL_VERIFY_NONE, MBEDTLS_SSL_VERIFY_OPTIONAL,
 * MBEDTLS_SSL_VERIFY_REQUIRED, MBEDTLS_SSL_VERIFY_UNSET
 *
 * @return		Pointer to MBEDTLS SSL configuration context
 */
mbedtls_ssl_config *wm_mbedtls_ssl_config_new(wm_mbedtls_cert_t *cert,
		int endpoint, int authmode);

/**
 * Free MBEDTLS SSL configuration context
 *
 * This function will free memory for following things:
 *
 * Allocations done by MBEDTLS itself internally
 * SSL configuration context allocated by WMSDK
 *
 * @param[in] conf		MBEDTLS SSL configuration context
 */
void wm_mbedtls_ssl_config_free(mbedtls_ssl_config *conf);

/**
 * Create MBEDTLS SSL context
 *
 * This function creates a MBEDTLS SSL context as per input
 * parameter MBEDTLS SSL configuration context, TCP socket
 * file descriptor and hostname to which SSL connection will
 * be done.
 *
 * Memory allocated for SSL context and MBEDTLS setup is done with
 * configuration context.
 *
 * Underlying BIO callbacks for write, read and read-with-timeout
 * along with socket file descriptor are set.
 *
 * Memory allocated for SSL internal timer, and set timer callbacks
 *
 * @param[in] conf		MBEDTLS SSL configuration context. This context
 * can set using api \ref wm_mbedtls_config_new
 *
 * @param[in] fd		socket descriptor.
 * @param[in] hostname	string denoting destination server name
 *
 * @return		Pointer to MBEDTLS SSL context
 *
 */
mbedtls_ssl_context *wm_mbedtls_ssl_new(mbedtls_ssl_config *conf,
		int fd, const char *hostname);

/**
 * Free MBEDTLS SSL context
 *
 * This function will free memory for following things:
 *
 * Internal timer for SSL context allocated by WMSDK
 * Allocations done by MBEDTLS itself internally
 * SSL context allocated by WMSDK
 *
 * @param[in] ssl		Pointer to MBEDTLS SSL context
 *
 */
void wm_mbedtls_ssl_free(mbedtls_ssl_context *ssl);

/**
 * Reset MBEDTLS SSL internal timer
 *
 * This function resets MBEDTLS SSL internal timer. This function call
 * is to done before every new call of 'mbedtls_ssl_read'. Reason behind this
 * is that, in each 'mbedtls_ssl_read' call, if timer is not running, then
 * MBEDTLS starts the timer with internal read timeout value (which can
 * be set using \ref wm_mbedtls_set_read_timeout).
 *
 * In 'mbedtls_ssl_read', timer status check is done before invoking functions
 * network layer read function (timer needs to be running i.e. status
 * should be 'not expired').
 *
 * After 'mbedtls_ssl_read' call is finished (in success or failure),
 * the timer is still running.
 *
 * If 'mbedtls_ssl_read' is again called, then 2 things
 * are possible, either the timer is expired or not (since the timer is
 * not reset i.e. it is still running from the time when first call to
 * 'mbedtls_ssl_read' was done)
 *
 * If timer is not expired then its fine, 'mbedtls_ssl_read' call will
 * proceed further as expected. But in case timer has expired then
 * 'mbedtls_ssl_read' will return SSL timeout error.
 *
 * @param[in] ssl		Pointer to MBEDTLS SSL context
 *
 */
void wm_mbedtls_reset_read_timer(mbedtls_ssl_context *ssl);

/**
 * Set read_timeout for MBEDTLS SSL read
 *
 * This function will set read timeout value, which is used
 * by MBEDTLS for setting internal timer. The status of this timer
 * is checked by MBEDTLS before calling network layer read function.
 *
 * @param[in] ssl		Pointer to MBEDTLS SSL context
 * @param[in] timeout	Desired timeout value to be set
 */
void wm_mbedtls_set_read_timeout(mbedtls_ssl_context *ssl, uint32_t timeout);

/**
 * Start SSL connection
 *
 * This function performs SSL handshake on existing TCP connection
 * along with verification of data returned by server as per user
 * specified verification flags provided at \ref wm_mbedtls_ssl_new
 *
 * @param[in] ssl	Pointer to MBEDTLS SSL context
 *
 * @return 0		Success
 * @return Non-zero	Failure
 */
int wm_mbedtls_ssl_connect(mbedtls_ssl_context *ssl);

#endif /* WM_MBEDTLS_HELPER_H */
