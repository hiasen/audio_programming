#include <pulse/pulseaudio.h>
#include "stdio_playback.h"
#include <stdio.h>


pa_sample_spec ss = {
	.rate = 44100,
	.channels = 2,
	.format = PA_SAMPLE_S16LE
};


void context_state_cb(pa_context *c, void *userdata) {
	pa_context_state_t state = pa_context_get_state(c);
	FILE *file = (FILE *) userdata;

	switch (state) {
        case PA_CONTEXT_READY:
			if (start_playback(c, file, &ss) < 0){
				fprintf(stderr, "Couldn't open stream\n");
				exit(1);
			}
			break;
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
		case PA_CONTEXT_FAILED:
		case PA_CONTEXT_TERMINATED:
        default:
            break;
	}

}

int main(int argc, char **argv) {
	FILE *file = stdin;
	if (argc >= 2){
		char *filename = argv[1];
		file = fopen(filename, "r");
		if (file == NULL) {
			fprintf(stderr, "Couldn't open file %s\n", filename);
			exit(1);
		}
	}

	pa_mainloop *ml = pa_mainloop_new();
	pa_mainloop_api *ml_api = pa_mainloop_get_api(ml);
	pa_context *c = pa_context_new(ml_api, "My application");

	pa_context_connect(c, NULL, PA_CONTEXT_NOFLAGS, NULL);
	pa_context_set_state_callback(c, context_state_cb, file);

	if (pa_mainloop_run(ml, NULL) < 0){
		fprintf(stderr, "error\n");
	}
	pa_mainloop_free(ml);
}
