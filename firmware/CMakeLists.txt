cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(opl2sd1)

pico_sdk_init()

add_executable(opl2sd1
    src/main.cpp
    src/OPLRegisterSet.cpp
    src/OPLTone.cpp
    src/OPLUtils.cpp
    src/SD1Device.cpp
    src/SD1OPLAdaptor.cpp
    src/SD1Tone.cpp
    src/SD1Utils.cpp
    src/SerialDecoder.cpp
    src/SerialProtocol.cpp
    src/usb_descriptors.c
    src/VoiceAllocator.cpp
)

pico_add_extra_outputs(opl2sd1)

target_include_directories(opl2sd1 PRIVATE src)

target_link_libraries(opl2sd1 pico_stdlib hardware_spi tinyusb_device)

pico_enable_stdio_usb(opl2sd1 0)
pico_enable_stdio_uart(opl2sd1 0)
