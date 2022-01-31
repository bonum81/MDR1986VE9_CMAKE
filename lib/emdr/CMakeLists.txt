add_library(stddrv STATIC
	CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.S
	CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/system_MDR32F9Qx.c
	#MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_adc.c
	#MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_dac.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_eeprom.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_port.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_can.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_rst_clk.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_ssp.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_timer.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_uart.c
	MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_usb.c
	MDR32F9Qx_StdPeriph_Driver/src/USB_Library/MDR32F9Qx_usb_CDC.c
	MDR32F9Qx_StdPeriph_Driver/src/USB_Library/MDR32F9Qx_usb_device.c
)

target_include_directories(stddrv PUBLIC
	Config
	CMSIS/CM3/CoreSupport
	CMSIS/CM3/DeviceSupport/MDR32F9Qx/inc
	CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup
	MDR32F9Qx_StdPeriph_Driver/inc
	MDR32F9Qx_StdPeriph_Driver/inc/USB_Library
)


# Compiler flags
#target_compile_options(stddrv PRIVATE
#	-mcpu=cortex-m3
#	-mthumb
#	-Wall
#	-Wextra
#	-Wshadow
#	-ffunction-sections
#	-fdata-sections
#)


# set(COMMON_FLAGS "-Wall -Wextra -Wshadow")
# set(COMMON_FLAGS "${COMMON_FLAGS} -ffunction-sections -fdata-sections")

# set(RELEASE_FLAGS "${ARCH_FLAGS} ${OPT_FLAGS} ${COMMON_FLAGS}")
# set(DEBUG_FLAGS "${ARCH_FLAGS} ${N_OPT_FLAGS} ${COMMON_FLAGS}")

# if(CMAKE_BUILD_TYPE STREQUAL "")
	# set(CMAKE_ASM_FLAGS "${DEBUG_FLAGS}")
	# set(CMAKE_C_FLAGS "${DEBUG_FLAGS}")
# endif()

# set(CMAKE_ASM_FLAGS_RELEASE "${RELEASE_FLAGS}")
# set(CMAKE_ASM_FLAGS_DEBUG "${DEBUG_FLAGS}")
# set(CMAKE_C_FLAGS_RELEASE "${RELEASE_FLAGS}")
# set(CMAKE_C_FLAGS_DEBUG "${DEBUG_FLAGS}")

