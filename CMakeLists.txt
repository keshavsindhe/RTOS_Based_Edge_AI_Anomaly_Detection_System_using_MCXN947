# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

set(app_board_core_dir edge_ai_vibration_cm33_core0)

cmake_path(APPEND conf_file_path ${CMAKE_CURRENT_LIST_DIR} ${app_board_core_dir} prj.conf)
set(CONF_FILE ${conf_file_path} ${CONF_FILE})
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(edge_ai_vibration LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/hello_world)

add_compile_definitions(I2C_RETRY_TIMES=1000000)


include(${SdkRootDirPath}/CMakeLists.txt)


mcux_add_source(
    SOURCES app/main_sensor.c app/ai_classifier.c app/task_scheduler.c app/task_signal.c app/task_fft.c app/task_feature.c app/task_ai.c dsp/signal_generator.c dsp/fft_engine.c dsp/feature_extraction.c dsp/feature_extractor.c
)

mcux_add_include(
    INCLUDES ${app_board_core_dir}/cm33_core0
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_include(INCLUDES .)
