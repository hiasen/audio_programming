/*
 * Example program using alsa sequencer
 */
#include <stdio.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

int main()
{
    snd_seq_t *handle;
    int err;
    err = snd_seq_open(&handle, "default", SND_SEQ_OPEN_INPUT, 0);
    if (err < 0) {
            fprintf(stderr, "%s\n", snd_strerror(-err));
            exit(1);
    }
    snd_seq_set_client_name(handle, "Yo!");
    int port_number = snd_seq_create_simple_port(handle, "my port",
            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    snd_seq_event_t *event;
    for (;;){
        snd_seq_event_input(handle, &event);
        printf("Type: %d\n", event->type);
        if (snd_seq_ev_is_note_type(event)){
            printf("Note: %d\n", event->data.note.note);
            printf("Velocity: %d\n", event->data.note.velocity);
        }
        printf("Source: %d:%d\n", event->source.client, event->source.port);
        printf("Destination: %d:%d\n", event->dest.client, event->dest.port);
    }

    return 0;
}


