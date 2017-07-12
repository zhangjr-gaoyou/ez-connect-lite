/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "aws_iot_error.h"
#ifdef _ENABLE_THREAD_SUPPORT_
#include <threads_interface.h>
/**
 * @brief Initialize the provided mutex
 *
 * Call this function to initialize the mutex
 *
 * @param IoT_Mutex_t - pointer to the mutex to be initialized
 * @return IoT_Error_t - error code indicating result of operation
 */
IoT_Error_t aws_iot_thread_mutex_init(IoT_Mutex_t *pMutex) {
	if (os_mutex_create(&pMutex->lock, "aws-mutex", OS_MUTEX_INHERIT)
	    != WM_SUCCESS) {
		return MUTEX_LOCK_ERROR;
	}
	return AWS_SUCCESS;
}

/**
 * @brief Lock the provided mutex
 *
 * Call this function to lock the mutex before performing a state change
 * Blocking, thread will block until lock request fails
 *
 * @param IoT_Mutex_t - pointer to the mutex to be locked
 * @return IoT_Error_t - error code indicating result of operation
 */
IoT_Error_t aws_iot_thread_mutex_lock(IoT_Mutex_t *pMutex) {
	if (os_mutex_get(&pMutex->lock, OS_WAIT_FOREVER) != WM_SUCCESS) {
		return MUTEX_LOCK_ERROR;
	}
	return AWS_SUCCESS;
}

/**
 * @brief Try to lock the provided mutex
 *
 * Call this function to attempt to lock the mutex before performing a state change
 * Non-Blocking, immediately returns with failure if lock attempt fails
 *
 * @param IoT_Mutex_t - pointer to the mutex to be locked
 * @return IoT_Error_t - error code indicating result of operation
 */
IoT_Error_t aws_iot_thread_mutex_trylock(IoT_Mutex_t *pMutex) {
	if (os_mutex_get(&pMutex->lock, OS_NO_WAIT) != WM_SUCCESS) {
		return MUTEX_LOCK_ERROR;
	}
	return AWS_SUCCESS;
}

/**
 * @brief Unlock the provided mutex
 *
 * Call this function to unlock the mutex before performing a state change
 *
 * @param IoT_Mutex_t - pointer to the mutex to be unlocked
 * @return IoT_Error_t - error code indicating result of operation
 */
IoT_Error_t aws_iot_thread_mutex_unlock(IoT_Mutex_t *pMutex) {
	if (os_mutex_put(&pMutex->lock) != WM_SUCCESS) {
		return MUTEX_LOCK_ERROR;
	}
	return AWS_SUCCESS;
}

/**
 * @brief Destroy the provided mutex
 *
 * Call this function to destroy the mutex
 *
 * @param IoT_Mutex_t - pointer to the mutex to be destroyed
 * @return IoT_Error_t - error code indicating result of operation
 */
IoT_Error_t aws_iot_thread_mutex_destroy(IoT_Mutex_t *pMutex) {
	if (os_mutex_delete(&pMutex->lock) != WM_SUCCESS) {
		return MUTEX_LOCK_ERROR;
	}
	return AWS_SUCCESS;
}

#endif /* _ENABLE_THREAD_SUPPORT_ */

