/*
 *  Copyright 2008-2017 Marvell International Ltd.
 *  All Rights Reserved.
 */
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <string.h>
#include <wmsdk.h>
#include <wm_mbedtls_helper_api.h>

#include "aws_iot_error.h"
#include "network_interface.h"

#define NET_BLOCKING_OFF 1
#define NET_BLOCKING_ON	0

static wm_tls_handle_t tls_hd;

static inline int net_socket_blocking(int sock, int state)
{
	return lwip_ioctl(sock, FIONBIO, &state);
}

static void Populate_ConnectParams(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
		 char *pDevicePrivateKeyLocation, char *pDestinationURL,
		 uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag)
{
	pNetwork->tlsConnectParams.DestinationPort = destinationPort;
	pNetwork->tlsConnectParams.pDestinationURL = pDestinationURL;
	pNetwork->tlsConnectParams.pDeviceCertLocation = pDeviceCertLocation;
	pNetwork->tlsConnectParams.pDevicePrivateKeyLocation = pDevicePrivateKeyLocation;
	pNetwork->tlsConnectParams.pRootCALocation = pRootCALocation;
	pNetwork->tlsConnectParams.timeout_ms = timeout_ms;
}

IoT_Error_t iot_tls_init(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
			 char *pDevicePrivateKeyLocation, char *pDestinationURL,
			 uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag)
{

	/* Initialize mbedtls library for mbedtls APIs */
	wm_mbedtls_lib_init();
	Populate_ConnectParams(pNetwork, pRootCALocation, pDeviceCertLocation,
			       pDevicePrivateKeyLocation, pDestinationURL,
			       destinationPort, timeout_ms, ServerVerificationFlag);

	pNetwork->my_socket = 0;
	pNetwork->connect = iot_tls_connect;
	pNetwork->read = iot_tls_read;
	pNetwork->write = iot_tls_write;
	pNetwork->disconnect = iot_tls_disconnect;
	pNetwork->isConnected = iot_tls_is_connected;
	pNetwork->destroy = iot_tls_destroy;

	return AWS_SUCCESS;
}

int Create_TCPSocket(void) {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	return sockfd;
}

static void Close_TCPSocket(int *sockfd) {
	if (-1 == *sockfd)
		return;
	close(*sockfd);
	*sockfd = -1;
}

IoT_Error_t Connect_TCPSocket(int socket_fd, char *pURLString, int port) {

	IoT_Error_t ret_val = TCP_CONNECTION_ERROR;
	int connect_status = -1;
	struct hostent *host;
	struct sockaddr_in dest_addr;

	host = gethostbyname(pURLString);

	if (NULL != host) {
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(port);
		dest_addr.sin_addr.s_addr = *(long*) (host->h_addr);
		memset(&(dest_addr.sin_zero), '\0', 8);

		connect_status = connect(socket_fd, (struct sockaddr *) &dest_addr,
				sizeof(struct sockaddr));
		if (-1 != connect_status) {
			ret_val = AWS_SUCCESS;
		}
	}
	return ret_val;
}

IoT_Error_t setSocketToNonBlocking(int server_fd) {
	net_socket_blocking(server_fd, NET_BLOCKING_OFF);
	return 0;
}

IoT_Error_t iot_tls_connect(Network *pNetwork, TLSConnectParams *params)
{
	if (pNetwork == NULL)
		return NULL_VALUE_ERROR;

	IoT_Error_t ret_val = AWS_FAILURE;

	if (params != NULL) {
		Populate_ConnectParams(pNetwork, params->pRootCALocation,
				       params->pDeviceCertLocation,
				       params->pDevicePrivateKeyLocation,
				       params->pDestinationURL,
				       params->DestinationPort,
				       params->timeout_ms,
				       params->ServerVerificationFlag);
	}

	pNetwork->my_socket = Create_TCPSocket();
	if (-1 == pNetwork->my_socket) {
		ret_val = TCP_CONNECTION_ERROR;
		return ret_val;
	}

	ret_val = Connect_TCPSocket(pNetwork->my_socket,
				    pNetwork->tlsConnectParams.pDestinationURL,
				    pNetwork->tlsConnectParams.DestinationPort);
	if (AWS_SUCCESS != ret_val) {
		Close_TCPSocket(&pNetwork->my_socket);
		return ret_val;
	}
	memset(&tls_hd, 0, sizeof(tls_hd));
	ret_val = wm_tls_client_open(&tls_hd,
	 (wm_tls_cert_t *) pNetwork->tlsConnectParams.pRootCALocation,
	 (wm_tls_cert_t *) pNetwork->tlsConnectParams.pDeviceCertLocation,
	 (wm_tls_key_t *) pNetwork->tlsConnectParams.pDevicePrivateKeyLocation,
	 pNetwork->my_socket);
	if (ret_val != WM_SUCCESS)
		goto out;

	return WM_SUCCESS;
out:
	wm_tls_client_close(&tls_hd);
	Close_TCPSocket(&pNetwork->my_socket);
	return -WM_FAIL;

}

IoT_Error_t iot_tls_read(Network *pNetwork, unsigned char *pMsg, size_t len,
			 Timer *timer, size_t *read_len)
{
	int val = -1;
	int recv_len = 0;
	int error_flag = false, complete_flag = false;
	unsigned int timeout = timer->timeout;

	wm_tls_reset_read_timer(&tls_hd);
	wm_tls_set_read_timeout(&tls_hd, timeout);

	do {
		val = wm_tls_client_read(&tls_hd, pMsg + recv_len,
			       len - recv_len);
		if (val >= 0) {
			recv_len += val;
		} else {
			error_flag = true;
		}

		if (recv_len >= len) {
			complete_flag = true;
		}
	} while (!error_flag && !complete_flag);

	*read_len = recv_len;
	if (0 == *read_len && error_flag) {
		return NETWORK_SSL_NOTHING_TO_READ;
	} else if (has_timer_expired(timer) && !complete_flag) {
		return NETWORK_SSL_READ_TIMEOUT_ERROR;
	}

	return AWS_SUCCESS;
}

IoT_Error_t iot_tls_write(Network *pNetwork, unsigned char *pMsg, size_t len,
			  Timer *timer, size_t *written_len)
{
	*written_len = wm_tls_client_write(&tls_hd, pMsg, len);
	if (*written_len >= 0)
		return AWS_SUCCESS;
	else
		return AWS_FAILURE;
}

IoT_Error_t iot_tls_disconnect(Network *pNetwork)
{
	wm_tls_client_close(&tls_hd);
	Close_TCPSocket(&pNetwork->my_socket);

	return AWS_SUCCESS;
}

IoT_Error_t iot_tls_destroy(Network *pNetwork)
{
	return AWS_SUCCESS;
}

IoT_Error_t iot_tls_is_connected(Network *pNetwork)
{
	return  NETWORK_PHYSICAL_LAYER_CONNECTED;
}
