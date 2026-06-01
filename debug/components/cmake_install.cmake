# Install script for directory: C:/Users/User/OneDrive/Desktop/embedded_ai/sdks/frdm_mcxn947_blank_sdk/mcuxsdk/components

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/edge_ai_vibration")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/User/.mcuxpressotools/arm-gnu-toolchain-14.2.rel1-mingw-w64-x86_64-arm-none-eabi/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/assert/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/audio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/aws_iot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/common_task/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/coremark/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/debug_console/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/debug_console_lite/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/display/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/exception_handling/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/internal_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/led/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/lists/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/log/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mem_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/messaging/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/misc_utilities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/osa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/panic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/pmic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/power_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/reset/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/reset1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/rpmsg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/sensor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/serial_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/shell/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/str/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/timer_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/time_stamp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/touch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/video/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/phy/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/IS42SM16800H/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/adc_sensor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/conn_fwloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/ele_crypto/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/ele_hseb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/i3c_bus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/imx_sm_crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/imu_adapter/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mt48lc2m32b2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mt48lc4m16a2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mx25_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mx25l_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mx25r_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/rtt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/scmi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/sdu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/slcd_engine/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/smt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/srtm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/sx1502/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/wifi_bt_module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/smbus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/debug_console_rtt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/notifier/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/ele_base_api/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/mpi_loader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/pinctrl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/clock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/silicon_id/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/sm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/systick_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/timer_lptmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/unity/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/power/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/codec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/expander/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/edgefast_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/eeprom_emulation/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/storage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/debug/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/gen_hal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/lce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_dspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_ecspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_enet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_flexcomm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_ii2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_iuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpc_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpc_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpc_vspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpc_vusart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpi2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpsci/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_lpuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/cmsis_drivers/cmsis_uart/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/User/OneDrive/Desktop/embedded_ai/edge_ai_vibration/debug/components/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
