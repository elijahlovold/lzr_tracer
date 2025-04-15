cmake_minimum_required(VERSION 3.13)

# Initialize the SDK
include(pico_sdk_import.cmake)

# Set the project name
project(lzr_cat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(lzr_cat)

add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Find all source files in the src directory
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/*.c
)

# Add the sources to the target
target_sources(lzr_cat PUBLIC ${SOURCES})

target_include_directories(lzr_cat PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include)

# Enable USB output (optional, for debugging)
pico_enable_stdio_usb(lzr_cat 1)
pico_enable_stdio_uart(lzr_cat 0)

# Link the necessary libraries
target_link_libraries(lzr_cat PUBLIC
    pico_stdlib 
    hardware_pwm
    hardware_adc
    FatFs_SPI
)

pico_add_extra_outputs(lzr_cat)
