#ifndef AUDIO_PROGRAMMING_STDIO_PLAYBACK_H
#define AUDIO_PROGRAMMING_STDIO_PLAYBACK_H
#include <stdio.h>
#include <pulse/pulseaudio.h>

int start_playback(pa_context *c, FILE *file, pa_sample_spec *ss);

#endif //AUDIO_PROGRAMMING_STDIO_PLAYBACK_H
