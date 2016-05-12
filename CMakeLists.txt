project("Audio programming" LANGUAGES C)
cmake_minimum_required(VERSION 3.4)

find_package(PulseAudio)

add_library(pcm16s pcm16s.c)
target_link_libraries(pcm16s m)

add_library(stdio_playback stdio_playback.c)
target_link_libraries(stdio_playback pulse)

add_executable(pa_client pa_client.c)
target_link_libraries(pa_client pulse stdio_playback)

add_subdirectory(simple_pa)
