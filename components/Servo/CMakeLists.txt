set(SERVO_DIR "${CMAKE_SOURCE_DIR}/components/Servo")

file(GLOB SERVO_SOURCES
    ${SERVO_DIR}/*.c
    ${SERVO_DIR}/*.cpp
)

add_library(Servo-Static
    ${SERVO_SOURCES}
)

target_include_directories(Servo-Static PUBLIC
    $<BUILD_INTERFACE:
        ${SERVO_DIR}
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(Servo-Static PUBLIC DriverLib-Static FreeRTOS-Static)

target_link_libraries(final.elf Servo-Static)
target_link_libraries(final.axf Servo-Static)
