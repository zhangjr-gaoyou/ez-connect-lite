# Copyright (C) 2008-2017, Marvell International Ltd.
# All Rights Reserved.


libs-y += libsensors

sensors_src_dir := $(d)/src

# Sensor interface and abstraction layer
libsensors-objs-y := src/sensor_drv.c

#ifeq ($(SEN_OCC),y) || ($(SEN_TEMPR),y) || ($(SEN_LIGHT),y) ||
#	($(SEN_TH),y) || ($(SEN_CO2),y) || ($(SEN_GAS),y) ||
#	($(SEN_PRESSURE),y) || ($(SEN_ACC),y) || ($(SEN_ULTRASONIC),y)

global-cflags-y += -DSENSORS_SUPPORTED=1
#endif

ifeq ($(SEN_OCC),y)
global-cflags-y += -DSEN_OCC=1
libsensors-objs-y += src/generic/sensor_occ_drv.c
endif

ifeq ($(SEN_LIGHT),y)
global-cflags-y += -DSEN_LIGHT=1
libsensors-objs-y += src/grove/sensor_light_drv.c
endif

ifeq ($(SEN_PRESSURE),y)
global-cflags-y += -DSEN_PRESSURE=1
libsensors-objs-y += src/grove/sensor_pressure_drv.c
endif

ifeq ($(SEN_TH),y)
global-cflags-y += -DSEN_TH=1
libsensors-objs-y += src/generic/dht_drv.c
libsensors-objs-y += src/grove/sensor_th_drv.c
endif

ifeq ($(SEN_GAS),y)
global-cflags-y += -DSEN_GAS=1
libsensors-objs-y += src/grove/sensor_gas_drv.c
endif

ifeq ($(SEN_CO2),y)
global-cflags-y += -DSEN_CO2=1
libsensors-objs-y += src/grove/sensor_co2_drv.c
endif

ifeq ($(SEN_TEMPR),y)
global-cflags-y += -DSEN_TEMPR=1
libsensors-objs-y += src/grove/sensor_tempr_drv.c
endif

ifeq ($(SEN_ACC),y)
global-cflags-y += -DSEN_ACC=1
libsensors-objs-y += src/grove/sensor_acc_drv.c
endif

ifeq ($(SEN_ULTRASONIC),y)
global-cflags-y += -DSEN_ULTRASONIC=1
libsensors-objs-y += src/grove/sensor_ultrasonic_drv.c
endif

libsensors-objs-y := $(libsensors-objs-y:$(d)/%=%)

global-cflags-y += -I$(d)/incl -I$(d)/incl/grove -I$(d)/incl/generic

libsensors-supported-toolchain-y := arm_gcc
