#include <stdio.h>
#include <string.h>

#include <alsa/asoundlib.h>
#include <glib.h>

#define RATE 44100
#define CHANNELS 2
#define FORMAT SND_PCM_FORMAT_S16_LE
#define FRAMES_TO_READ 441000

static void set_hw_params(snd_pcm_t *pcm);
static void play_pcm(snd_pcm_t *pcm, FILE *file);


int main(int argc, char**argv){
    FILE *file = stdin;
    if (argc >= 2){
        file = fopen(argv[1], "r");
        if (argc >= 3) {
            int seek = atoi(argv[2]);
            fseek(file, seek, 0);
        }
    }
    snd_pcm_t *pcm;
    if(snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0){
        g_error("Failed to open pcm device");
    }

    set_hw_params(pcm);
    play_pcm(pcm, file);
    snd_pcm_close(pcm);
    return 0;
}

static void set_hw_params(snd_pcm_t *pcm) {
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_alloca(&hw_params);

    if (snd_pcm_hw_params_any(pcm, hw_params) < 0) {
        g_error("Failed to get configuration space for hw_params");
    }

    // --- Setting wanted hardware parameters
    if (snd_pcm_hw_params_set_rate(pcm, hw_params, RATE, 0) < 0) {
        g_error("Couldn't set wanted rate: %d", RATE);
    }
    if (snd_pcm_hw_params_set_format(pcm, hw_params, FORMAT) < 0) {
        g_error("Couldn't set wanted format");
    }
    if (snd_pcm_hw_params_set_channels(pcm, hw_params, CHANNELS) < 0) {
        g_error("Couldn't set wanted channel count %d", CHANNELS);
    }

    if (snd_pcm_hw_params_set_channels(pcm, hw_params, CHANNELS) < 0) {
        g_error("Couldn't set wanted channel count %d", CHANNELS);
    }

    // --- Set the hardware parameters to the hardware
    if (snd_pcm_hw_params(pcm, hw_params) < 0) {
        g_error("Failed to set hw_params");
    }
}

static void play_pcm(snd_pcm_t *pcm, FILE *file) {
    size_t bytes_per_frame = (size_t) snd_pcm_frames_to_bytes(pcm, 1);
    size_t buf_size = (size_t) snd_pcm_frames_to_bytes(pcm, FRAMES_TO_READ);

    void *buf = malloc(buf_size);
    while (!feof(file)){
        size_t frames_read = fread(buf, bytes_per_frame, FRAMES_TO_READ, file);
        if (ferror(file)){
            g_error("Error reading stream");
        }
        snd_pcm_sframes_t frames_written = snd_pcm_writei(pcm, buf, frames_read);
        if (frames_written < 0){
            g_error("Error writing to audio device\n");
        } else if (frames_written < frames_read){
            g_error("Didn't write all that was read from file %d/%d\n", frames_written, frames_read);
        }
    }
    free(buf);
    snd_pcm_drain(pcm);
}
