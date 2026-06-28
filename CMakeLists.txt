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


# PART 11 - TinyML Integration Build (Pipeline Structure with STUB Inference)
# Uses stub TinyML engine for testing pipeline without TensorFlow Lite library
# Full pipeline: Signal → FFT → Feature → TinyML(stub) → UART
# To enable real TFLite: replace tinyml_engine_stub.c with tinyml_engine.c + model_data.cc
mcux_add_source(
    SOURCES 
    app/main_tinyml.c 
    app/task_scheduler.c 
    app/task_signal.c 
    app/task_fft.c 
    app/task_feature.c 
    dsp/signal_generator.c 
    dsp/fft_engine.c 
    dsp/feature_extraction.c 
    dsp/feature_extractor.c
    ml/tinyml_task.c
    ml/tinyml_engine_stub.c    # STUB version (no TFLite dependency)
    # ml/tinyml_engine.c        # Real TFLite version (requires TensorFlow Lite Micro)
    # ml/model_data.cc           # Trained model weights
)

mcux_add_include(
    INCLUDES ${app_board_core_dir}/cm33_core0
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_include(INCLUDES .)
