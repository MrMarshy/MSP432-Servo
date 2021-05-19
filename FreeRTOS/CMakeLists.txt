set(FreeRTOS_SRC_DIR "${CMAKE_SOURCE_DIR}/FreeRTOS")
set(FreeRTOS_PORTABLE_DIR "${CMAKE_SOURCE_DIR}/FreeRTOS/Source/portable")


file(GLOB FREERTOS_SOURCES 
    ${FreeRTOS_SRC_DIR}/Source/*.c 
    ${FreeRTOS_PORTABLE_DIR}/Common/*.c
    ${FreeRTOS_PORTABLE_DIR}/GCC/ARM_CM4F/*.c
    ${FreeRTOS_PORTABLE_DIR}/UsedMemMang/*.c
)

add_library(FreeRTOS-Static STATIC
    ${FREERTOS_SOURCES}
)

target_include_directories(FreeRTOS-Static PUBLIC
        $<BUILD_INTERFACE:
            ${FreeRTOS_SRC_DIR}
            ${FreeRTOS_SRC_DIR}/Source/include
            ${FreeRTOS_PORTABLE_DIR}/GCC/ARM_CM4F>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(FreeRTOS-Static PUBLIC DriverLib-Static)

target_link_libraries(final.elf FreeRTOS-Static)
target_link_libraries(final.axf FreeRTOS-Static)

