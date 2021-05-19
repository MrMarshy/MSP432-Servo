set(TCS3200_DIR "${CMAKE_SOURCE_DIR}/components/TCS3200")

file(GLOB TCS3200_SOURCES
    ${TCS3200_DIR}/*.c
    ${TCS3200_DIR}/*.cpp
)

add_library(TCS3200-Static
    ${TCS3200_SOURCES}
)

target_include_directories(TCS3200-Static PUBLIC
    $<BUILD_INTERFACE:
        ${TCS3200_DIR}
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(TCS3200-Static PUBLIC DriverLib-Static FreeRTOS-Static)

target_link_libraries(final.elf TCS3200-Static)
target_link_libraries(final.axf TCS3200-Static)
