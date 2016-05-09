project("Audio programming" LANGUAGES C)
cmake_minimum_required(VERSION 3.4)

find_package(PulseAudio)

add_library(pcm16s pcm16s.c)
target_link_libraries(pcm16s m)

add_executable(simple_pa_client simple_pa_client.c)
target_link_libraries(simple_pa_client pcm16s pulse pulse-simple)

add_executable(simple_pa_recorder simple_pa_recorder.c)
target_link_libraries(simple_pa_recorder pulse pulse-simple)
