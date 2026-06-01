# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES frdmmcxn947/board.c
            frdmmcxn947/board.h
)

mcux_add_include(
    INCLUDES frdmmcxn947
)

mcux_add_source(
    SOURCES frdmmcxn947/clock_config.c
            frdmmcxn947/clock_config.h
)

mcux_add_include(
    INCLUDES frdmmcxn947
)
