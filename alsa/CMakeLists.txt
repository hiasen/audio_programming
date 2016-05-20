find_package(ALSA)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})


add_executable(list list_devices.c)
target_link_libraries(list asound)

add_executable(rawmidi rawmidi_read.c)
target_link_libraries(rawmidi asound)

add_executable(rawmidi_write rawmidi_write.c)
target_link_libraries(rawmidi_write asound)

add_executable(pcm_from_file pcm_from_file.c)
target_link_libraries(pcm_from_file asound ${GLIB2_LIBRARIES})
