find_package(ALSA)

add_executable(list list_devices.c)
target_link_libraries(list asound)

add_executable(rawmidi rawmidi_read.c)
target_link_libraries(rawmidi asound)

add_executable(rawmidi_write rawmidi_write.c)
target_link_libraries(rawmidi_write asound)
