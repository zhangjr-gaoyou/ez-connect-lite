/*
 *  Copyright (C) 2017, VADACTRO (www.vadactro.org.in).
 *  All Rights Reserved.
 */

/*
 * This is a board specific configuration file for
 * the VD-IOT-EVALKIT-WIFIB-V1P board
 *
 * Visit below url for more details
 * https://gitlab.com/VD-EDU/vd-iot-evalkit-sdk/wikis/home
 */

#include <wmtypes.h>
#include <wmerrno.h>
#include <wm_os.h>
#include <board.h>
#include <lowlevel_drivers.h>

/* Source module specific board functions for Murata CMWC1ZZABR module */
#include <modules/CMWC1ZZABR.c>

int board_cpu_freq()
{
	return 200000000;
}

int board_32k_xtal()
{
	return false;
}

int board_32k_osc()
{
	return false;
}

int board_rc32k_calib()
{
	return true;
}

void board_gpio_power_on()
{
}

void board_uart_pin_config(int id)
{
	switch (id) {
	case UART0_ID:
		GPIO_PinMuxFun(GPIO_2, GPIO2_UART0_TXD);
		GPIO_PinMuxFun(GPIO_3, GPIO3_UART0_RXD);
		break;
	case UART1_ID:
		break;
	case UART2_ID:
		GPIO_PinMuxFun(GPIO_9, GPIO9_UART2_TXD);
		GPIO_PinMuxFun(GPIO_10, GPIO10_UART2_RXD);
		break;
	}
}

void board_i2c_pin_config(int id)
{
	switch (id) {
	case I2C0_PORT:
		break;
	case I2C1_PORT:
		GPIO_PinMuxFun(GPIO_26, GPIO26_I2C1_SCL);
		GPIO_PinMuxFun(GPIO_25, GPIO25_I2C1_SDA);
		break;
	}
}

void board_usb_pin_config()
{
	GPIO_PinMuxFun(GPIO_27, GPIO27_DRVVBUS);
}

void board_ssp_pin_config(int id, bool cs)
{
	/* To do */
	switch (id) {
	case SSP0_ID:
		break;
	case SSP1_ID:
		break;
	case SSP2_ID:
		GPIO_PinMuxFun(GPIO_46, GPIO46_SSP2_CLK);
		GPIO_PinMuxFun(GPIO_47, GPIO47_SSP2_FRM);
		GPIO_PinMuxFun(GPIO_48, GPIO48_SSP2_TXD);
		GPIO_PinMuxFun(GPIO_49, GPIO49_SSP2_RXD);
		break;
	}
}

int board_adc_pin_config(int adc_id, int channel)
{
	/* Channel 2 and channel 3 need GPIO 44
	 * and GPIO 45 which are used for
	 * RF control and not available for ADC
	 */
	if (channel == ADC_CH2 || channel == ADC_CH3) {
		return -WM_FAIL;
	}
	GPIO_PinMuxFun((GPIO_42 + channel),
			 PINMUX_FUNCTION_1);
	return WM_SUCCESS;
}

void board_dac_pin_config(int channel)
{
	switch (channel) {
	case DAC_CH_A:
		/* For this channel GPIO 44 is needed
		 * GPIO 44 is reserved for  RF control
		 * on this module so channel DAC_CH_A
		 * should not be used.
		 */
		break;
	case DAC_CH_B:
		GPIO_PinMuxFun(GPIO_43, GPIO43_DACB);
		break;
	}
}

output_gpio_cfg_t board_led_1()
{
	output_gpio_cfg_t gcfg = {
		.gpio = GPIO_0, /* BLUE LED (nLINK) */
		.type = GPIO_ACTIVE_LOW,
	};
	return gcfg;
}

output_gpio_cfg_t board_led_2()
{
	output_gpio_cfg_t gcfg = {
		.gpio = GPIO_1, /* RED LED (POW_LED) */
		.type = GPIO_ACTIVE_LOW,
	};

	return gcfg;
}

output_gpio_cfg_t board_led_3()
{
	output_gpio_cfg_t gcfg = {
		.gpio = GPIO_4, /* GREEN LED (USR_LED) */
		.type = GPIO_ACTIVE_LOW,
	};

	return gcfg;
}

output_gpio_cfg_t board_led_4()
{
	output_gpio_cfg_t gcfg = {
		.gpio = -1,
	};

	return gcfg;
}

int board_button_1()
{
	GPIO_PinMuxFun(GPIO_39, GPIO39_GPIO39);
	return GPIO_39; /* TSW5 (Reset to Prov) */
}

int board_button_2()
{
	GPIO_PinMuxFun(GPIO_22, GPIO22_GPIO22);
	return GPIO_22; /* WAKE0 (User Pushbutton) */
}

int board_button_3()
{
	return -WM_FAIL;
}

int board_button_pressed(int pin)
{
	if (pin < 0)
		return false;

	GPIO_SetPinDir(pin, GPIO_INPUT);
	if (GPIO_ReadPinLevel(pin) == GPIO_IO_LOW)
		return true;

	return false;
}

int board_wakeup0_functional()
{
	return true;
}

int board_wakeup1_functional()
{
	return true;
}

unsigned int board_antenna_select()
{
	return 1;
}
