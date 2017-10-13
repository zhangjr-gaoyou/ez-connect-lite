/*
 *  Copyright (C) 2008-2017, Marvell International Ltd.
 *  All Rights Reserved.
 */

/*
 * This is a module specific configuration file for
 * the CMWC1ZZABR-xxx WiFi module from Murata
 */

#include <wmtypes.h>
#include <wmerrno.h>
#include <wm_os.h>
#include <board.h>
#include <lowlevel_drivers.h>

int board_main_xtal()
{
	/* MAINXTAL: 38.4MHZ */
	return 38400000;
}

int board_main_osc()
{
	return -WM_FAIL;
}

int board_antenna_switch_ctrl()
{
	return false;
}

struct pwr_table *board_region_pwr_tbl(board_country_code_t country)
{
	return NULL;
}
