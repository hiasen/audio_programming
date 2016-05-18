find_package(ALSA)

add_executable(list list_devices.c)
target_link_libraries(list asound)