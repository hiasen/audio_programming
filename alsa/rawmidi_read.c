#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <alsa/asoundlib.h>

#define BUFSIZE 1024

int main(){
    snd_rawmidi_t *inputp;
    void *buf;
    int err;
    ssize_t bytes_read;

    err = snd_rawmidi_open(&inputp, NULL, "default", 0);
    if (err < 0){
        fprintf(stderr, "Failed to open rawmidi\n");
        exit(1);
    }

    buf = malloc(BUFSIZE);
    for (int i = 0; i < 100; i++) {
        bytes_read = snd_rawmidi_read(inputp, buf, BUFSIZE);
        fprintf(stderr, "%d bytes read\n", (int) bytes_read);
        fwrite(buf, 1, (size_t) bytes_read, stdout);
        fflush(stdout);
    }

    free(buf);
    snd_rawmidi_close(inputp);
    return 0;
}