# Copyright (C) 2008-2017, Marvell International Ltd.
# All Rights Reserved.

libs-y += libmbedtls

mbedtls_upstream_dir := upstream/library/
mbedtls_helper_dir := helper_api/
mbedtls_port_dir := port/

# Crypto c files
libmbedtls_upstream-objs-y := \
		aes.c		aesni.c		arc4.c		\
		asn1parse.c	asn1write.c	base64.c	\
		bignum.c	blowfish.c	camellia.c	\
		ccm.c		cipher.c	cipher_wrap.c	\
		cmac.c		ctr_drbg.c	des.c		\
		dhm.c		ecdh.c		ecdsa.c		\
		ecjpake.c	ecp.c		ecp_curves.c	\
		error.c		gcm.c		havege.c	\
		hmac_drbg.c	md.c		md2.c		\
		md4.c		md5.c		md_wrap.c	\
		padlock.c	pem.c		pk.c		\
		pk_wrap.c	pkcs12.c	pkcs5.c		\
		pkparse.c	pkwrite.c	platform.c	\
		ripemd160.c	rsa.c		sha1.c		\
		sha256.c	sha512.c	threading.c	\
		version.c	entropy_poll.c	\
		oid.c		memory_buffer_alloc.c	\
		xtea.c		version_features.c		\
		entropy.c

# X509 c files
libmbedtls_upstream-objs-y += \
		x509.c		x509write_crt.c		\
		x509_crl.c	x509_crt.c			\
		x509_csr.c	x509_create.c		\
		certs.c		x509write_csr.c		\
		pkcs11.c

# TLS c files
libmbedtls_upstream-objs-y +=	\
		debug.c		\
		ssl_cache.c	ssl_ciphersuites.c	\
		ssl_cli.c	ssl_cookie.c		\
		ssl_srv.c	ssl_ticket.c		\
		ssl_tls.c

libmbedtls_helper_api-objs-y :=		\
	wm_mbedtls_helper_api.c

libmbedtls_port-objs-y :=	\
	timing_alt.c			\
	threading_alt.c			\
	wm_mbedtls_mem.c		\
	wm_mbedtls_net.c		\
	wm_mbedtls_entropy.c

# Prepend the source directory path
libmbedtls-objs-y := $(addprefix $(mbedtls_upstream_dir),$(libmbedtls_upstream-objs-y))
libmbedtls-objs-y += $(addprefix $(mbedtls_helper_dir),$(libmbedtls_helper_api-objs-y))
libmbedtls-objs-y += $(addprefix $(mbedtls_port_dir),$(libmbedtls_port-objs-y))

global-cflags-y += \
	-I$(d)/upstream/include \
	-I$(d)/port \
	-I$(d)/helper_api

# if MBEDTLS_CONFIG_FILE is not defined
# default upstream/include/mbedtls/config.h will be included

MBEDTLS_CONFIG ?= default

ifeq ($(wildcard $(d)/config/$(MBEDTLS_CONFIG).h),)
  $(error "Specified MBEDTLS_CONFIG file $(MBEDTLS_CONFIG).h not present in $(d)/config/ folder")
endif

global-cflags-y += \
	-I$(d)/config \
	-DMBEDTLS_CONFIG_FILE='<$(MBEDTLS_CONFIG).h>'

libmbedtls-supported-toolchain-y := arm_gcc iar
