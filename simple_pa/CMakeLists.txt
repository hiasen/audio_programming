add_executable(waveform_player waveform_player.c)
target_link_libraries(waveform_player pcm16s pulse pulse-simple)

add_executable(simple_pa_recorder simple_pa_recorder.c)
target_link_libraries(simple_pa_recorder pulse pulse-simple)
