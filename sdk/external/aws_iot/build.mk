# Copyright (C) 2008-2017, Marvell International Ltd.
# All Rights Reserved.

global-cflags-y += -I$(d)/include

# To enable multithread support uncomment following line
global-cflags-y += -D_ENABLE_THREAD_SUPPORT_

-include $(d)/build.aws_iot.mk
