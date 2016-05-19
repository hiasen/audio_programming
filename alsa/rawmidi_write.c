/*
 * Write to a midi device
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <alsa/asoundlib.h>

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

int main(int argc, char *argv[]){
    char *name = "default";
    if (argc >=2){
        name = argv[1];
    }
    snd_rawmidi_t *outputp;

    if (snd_rawmidi_open(NULL, &outputp, "default", 0) < 0){
        fprintf(stderr, "Failed to open rawmidi\n");
        exit(1);
    }

    unsigned char *buf = calloc(1, 3);
    buf[0] = MIDI_NOTE_ON;
    buf[1] = 0; // note number
    buf[2] = 64; // velocity


    for (int i = 0; i < 128; ++i) {
        snd_rawmidi_write(outputp, buf, 3);
        usleep(200000);
        buf[0] = MIDI_NOTE_OFF;
        snd_rawmidi_write(outputp, buf, 3);
        buf[0] = MIDI_NOTE_ON;
        buf[1]++;
    }

    if(snd_rawmidi_drain(outputp) < 0) {
        fprintf(stderr, "Failed to drain\n");
    }
    free(buf);
    snd_rawmidi_close(outputp);
    return 0;
}