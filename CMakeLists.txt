cmake_minimum_required(VERSION 3.18.4)

project(final)
enable_language(C CXX)

set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")


if(CMAKE_CROSSCOMPILING) 
    message(STATUS "Crosscompiling enabled, using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
    message( STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}" )

    set(LDSCRIPT "${CMAKE_SOURCE_DIR}/msp432p401r.lds")

    add_definitions(
        -D__MSP432P401R__ 
        -DTARGET_IS_MSP432P4XX 
        -DNO_MSP_CLASSIC_DEFINES
    )

    set(FP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
    set(ARCH_FLAGS "-mthumb -mcpu=cortex-m4 ${FP_FLAGS}")
    set(CMAKE_ASM_FLAGS "-D__ASSEMBLY__ -mcpu=cortex-m4 -mthumb")
    set(CMAKE_C_FLAGS " \
    ${COMMON_C_FLAGS} \
    -fsingle-precision-constant -Wdouble-promotion -Wfloat-conversion \
    ${ARCH_FLAGS}")

    set(CMAKE_CXX_FLAGS " \
    ${COMMON_CXX_FLAGS} \
    -fsingle-precision-constant -Wdouble-promotion -Wfloat-conversion \
    ${ARCH_FLAGS}")

    set(CMAKE_EXE_LINKER_FLAGS " \
    ${ARCH_FLAGS} \
    -T${LDSCRIPT} \
    --specs=nano.specs \
    -g3 \
    -ggdb \
    -Wl,-Map=final.map")

    add_subdirectory(src)

    # Add Components Here - See FindRegisterComponents.cmake for options to include exclude packages
    find_package(RegisterComponents) # FindRegisterComponents.cmake
    find_package(Driverlib)
    find_package(FreeRTOS)
    find_package(Servo)
    find_package(TCS3200)
    find_package(TIRSLK)

    

else()
    message(STATUS "No crosscompiling specified, compiling in /tests only.")
    add_subdirectory(tests)
endif()



set(COMMON_C_FLAGS "-O0 -g3 -ggdb -std=gnu11 \
-Wall -Werror -Wstrict-prototypes \
-Wimplicit -Wcast-align -Wpointer-arith 
-ffunction-sections -fdata-sections -MD -Wundef")

set(COMMON_CXX_FLAGS "-O0 -g3 -Wall -Werror -std=c++17 -ggdb \
-fno-exceptions -fno-rtti \
-fno-threadsafe-statics -ffunction-sections -fdata-sections")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
