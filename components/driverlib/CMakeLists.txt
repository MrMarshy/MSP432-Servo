set(DRIVERLIB_DIR "${CMAKE_SOURCE_DIR}/components/driverlib")

file(GLOB DRIVERLIB_SOURCES
    ${DRIVERLIB_DIR}/*.c
)

add_library(DriverLib-Static
    ${DRIVERLIB_SOURCES}
)

target_include_directories(DriverLib-Static PUBLIC
        $<BUILD_INTERFACE:
            #${CMAKE_SOURCE_DIR}/include
            ${DRIVERLIB_DIR}
            ${DRIVERLIB_DIR}/inc 
            ${DRIVERLIB_DIR}/inc/CMSIS>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(final.elf DriverLib-Static)
target_link_libraries(final.axf DriverLib-Static)