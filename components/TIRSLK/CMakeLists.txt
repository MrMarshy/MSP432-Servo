set(TIRSLK_DIR "${CMAKE_SOURCE_DIR}/components/TIRSLK")

file(GLOB TIRSLK_SOURCES
    ${TIRSLK_DIR}/*.c
    ${TIRSLK_DIR}/*.cpp
)

add_library(TIRSLK-Static
    ${TIRSLK_SOURCES}
)

target_include_directories(TIRSLK-Static PUBLIC
    $<BUILD_INTERFACE:
        ${TIRSLK_DIR}
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(TIRSLK-Static PUBLIC DriverLib-Static FreeRTOS-Static)

target_link_libraries(final.elf TIRSLK-Static)
target_link_libraries(final.axf TIRSLK-Static)