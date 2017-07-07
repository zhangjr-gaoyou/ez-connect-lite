#ifndef TIMING_ALT_H
#define TIMING_ALT_H

#include <compat_time.h>
#include <wmtime.h>

#define mbedtls_timing_hr_time timeval

typedef struct {
	struct mbedtls_timing_hr_time	timer;
	uint32_t						int_ms;
	uint32_t						fin_ms;
} mbedtls_timing_delay_context;


unsigned long mbedtls_timing_hardclock(void);

unsigned long mbedtls_timing_get_timer(struct mbedtls_timing_hr_time *val,
		int reset);

void mbedtls_set_alarm(int seconds);

int mbedtls_timing_get_delay(void *data);
void mbedtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms);

#endif /* TIMING_ALT_H */
