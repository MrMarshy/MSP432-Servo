cmake_minimum_required(VERSION 3.18.4)

# set(FreeRTOS_SRC_DIR "${CMAKE_SOURCE_DIR}/FreeRTOS")
# set(FreeRTOS_PORTABLE_DIR "${CMAKE_SOURCE_DIR}/FreeRTOS/Source/portable")
# set(DRIVERLIB_DIR "${CMAKE_SOURCE_DIR}/components/driverlib")

file(GLOB USER_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(final.elf ${USER_SOURCES})
add_executable(final.axf ${USER_SOURCES})

target_include_directories(final.elf PUBLIC
    $<BUILD_INTERFACE:
        # ${FreeRTOS_SRC_DIR}
        # ${FreeRTOS_SRC_DIR}/Source/include
        # ${FreeRTOS_PORTABLE_DIR}/GCC/ARM_CM4F>
        ${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(final.axf PUBLIC
    $<BUILD_INTERFACE:
        # ${FreeRTOS_SRC_DIR}
        # ${FreeRTOS_SRC_DIR}/Source/include
        # ${FreeRTOS_PORTABLE_DIR}/GCC/ARM_CM4F>
        ${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(final.elf DriverLib-Static FreeRTOS-Static Servo-Static)
target_link_libraries(final.axf DriverLib-Static FreeRTOS-Static Servo-Static)

# Print out the firmware size after compiling and building
add_custom_target(
        print_firmware_size ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/final.axf
        COMMAND python ${CMAKE_SOURCE_DIR}/get-fw-size.py ${CMAKE_SOURCE_DIR}/build/src/final.elf 0x40000 0x10000
)

