cmake_minimum_required(VERSION 3.15.3)

project(MDR32 VERSION 0.0.1 LANGUAGES C CXX ASM)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#set(ARCH_FLAGS "-mcpu=cortex-m3 -mthumb")
#set(OPT_FLAGS "-O2")
#set(N_OPT_FLAGS "-O0 -ggdb3")


# Add preprocessor definitions to the compilation of source files
add_compile_definitions(USE_MDR1986VE9x)
add_compile_definitions(USE_JTAG_A)
add_compile_definitions(__START=main)
add_compile_definitions(__STARTUP_CLEAR_BSS)


add_compile_options(
	-march=armv7-m
	-mcpu=cortex-m3
	-mthumb
	-mfloat-abi=soft
	-O0
	-ggdb3					# info for gdb
	-Wall
	-Wextra
	-Wshadow
	#-Wsign-conversion
	-ffunction-sections
	-fdata-sections
	$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)


add_link_options(
	-T${CMAKE_SOURCE_DIR}/lib/emdr/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld
	-lc
	-lm
	-lnosys
	-lstdc++
	--xref
	--specs=nano.specs
	--specs=nosys.specs
	-Wl,--gc-sections
	-Wl,--print-memory-usage
	-Wl,--undefined=uxTopUsedPriority
	-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
)


#Add directory librares
add_subdirectory(lib)

#Add main includes directory
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/MAIN/include
	${CMAKE_CURRENT_SOURCE_DIR}/Core/ParamSys
	${CMAKE_CURRENT_SOURCE_DIR}/Core/ParamSys/ParamHandlers
	${CMAKE_CURRENT_SOURCE_DIR}/Peripheals/Timers
	${CMAKE_CURRENT_SOURCE_DIR}/Peripheals/CAN
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Protocols/
	${CMAKE_CURRENT_SOURCE_DIR}/Core/SwTimers/
)



#Add main src files
file(GLOB MainSRC ${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/*.cpp)
file(GLOB CoreParamSysSRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/ParamSys/*.cpp)
file(GLOB CoreSwTimersSRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/SwTimers/*.cpp)
file(GLOB CoreParamHandlersSRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/ParamSys/ParamHandlers/*.cpp)
file(GLOB CoreProtocolsSRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/Protocols/*.cpp)
file(GLOB PeriphealsTimersSRC ${CMAKE_CURRENT_SOURCE_DIR}/Peripheals/Timers/*.cpp)
file(GLOB PeriphealsCANSRC ${CMAKE_CURRENT_SOURCE_DIR}/Peripheals/CAN/*.cpp)

set(SRC ${MainSRC}
 		${CoreParamSysSRC}
		${CoreSwTimersSRC}
		${CoreParamHandlersSRC}
		${PeriphealsTimersSRC}
		${PeriphealsCANSRC}
		${CoreProtocolsSRC}
		)

	#${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/app.c
	#${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/init.c
	#${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/can.c
	#${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/ft_protocol.c
	#${CMAKE_CURRENT_SOURCE_DIR}/MAIN/src/paramsys.c

#Add freertos dir
#set(FREERTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS/)
#file(GLOB_RECURSE FREERTOS_SOURCES 
#	"${FREERTOS_DIR}/*.c"
#	"${FREERTOS_DIR}/portable/GCC/ARM_CM3/port.c"
#	"${FREERTOS_DIR}/portable/MemMang/heap_4.c"
#)

#include_directories(${FREERTOS_DIR}/include/)
#include_directories(${FREERTOS_DIR}/portable/GCC/ARM_CM3)


#add_executable(${PROJECT_NAME} ${SRC} ${FREERTOS_SOURCES})
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} stddrv)
target_link_libraries(${PROJECT_NAME} w5500)
#target_link_libraries(${PROJECT_NAME} freertos)

#set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
#set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED ON)


# function (createBinAndHexFiles TARGET_NAME)
    # add_custom_command(TARGET ${TARGET_NAME}
        # POST_BUILD
        # COMMAND ${CMAKE_SIZE_UTIL} ${TARGET_NAME}.out
        # COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME}.out "${TARGET_NAME}.bin"
        # COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET_NAME}.out "${TARGET_NAME}.hex"
        # COMMENT "post build steps for ${TARGET_NAME}"
    # )
# endfunction()

# createBinAndHexFiles(${PROJECT_NAME})

add_custom_target(hex
	ALL
	COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME} ${PROJECT_NAME}.elf
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
	COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
	DEPENDS ${PROJECT_NAME}
	COMMENT "Generating Hex file ${PROJECT_NAME}.hex"
)
