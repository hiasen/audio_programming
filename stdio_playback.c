#include "stdio_playback.h"

static void finished_playing(pa_stream*s, int success, void *userdata){
    pa_stream_disconnect(s);
}

static void stop_playing(pa_stream *s) {
    pa_stream_drain(s, finished_playing, NULL);
    pa_stream_set_write_callback(s, NULL, NULL);
}

static void write_cb(pa_stream *s, size_t bytes, void *userdata) {
    void *buf;
    size_t buf_size =  (size_t) -1;
    pa_stream_begin_write(s, &buf, &buf_size);

    FILE *file = userdata;
    size_t bytes_read = fread(buf, 1, buf_size, file);

    if (bytes_read == 0) {
        pa_stream_cancel_write(s);
        stop_playing(s);
    } else {
        pa_stream_write(s, buf, bytes_read, NULL, 0, PA_SEEK_RELATIVE);
    }
}

static void stream_state_cb(pa_stream *s, void *userdata) {
    pa_stream_state_t state = pa_stream_get_state(s);
    switch (state) {
        case PA_STREAM_TERMINATED:
            pa_stream_unref(s);
            break;
        case PA_STREAM_UNCONNECTED:
        case PA_STREAM_CREATING:
        case PA_STREAM_FAILED:
        default:
            break;
    }
}

int start_playback(pa_context *c, FILE *file, pa_sample_spec *ss) {
    pa_stream *stream = pa_stream_new(c, "stdio player", ss, NULL);
    if (stream == NULL) {
        fprintf(stderr, "Error creating stream.");
        return -1;
    }
    pa_stream_set_state_callback(stream, stream_state_cb, (void*) file);
    pa_stream_set_write_callback(stream, write_cb, file);
    pa_stream_connect_playback(stream, NULL, NULL, PA_STREAM_NOFLAGS, NULL, NULL);
    return 0;
}
