
#include <stdio.h>
#include <stdlib.h>
#include <pulse/error.h>
#include <pulse/simple.h>

#define RECORDING_TIME 5

pa_sample_spec ss = {
	.format = PA_SAMPLE_S16NE,
	.channels = 1,
	.rate = 48000
};

void record(char *data, size_t bytes){
	int error;
	pa_simple *s;

	s = pa_simple_new(
		NULL, // Server
		"Simple recorder", // Name
		PA_STREAM_RECORD, // stream direction
		NULL, // device
		"Recording", // Stream name
		&ss, // Sample spec
		NULL, // Channel map
		NULL, // buffering attributes
		&error
	);

	if (s == NULL) {
		fprintf(stderr, "Error opening for recording: %s", pa_strerror(error));
		exit(1);
	}
	printf("Now recording for %d seconds\n", RECORDING_TIME);
	pa_simple_read(s, data, bytes, NULL);
	pa_simple_free(s);
	printf("Finished recording\n");
}

void play(char *data, size_t bytes){
	int error;
	pa_simple *s;
	s = pa_simple_new(
		NULL, // Server
		"Simple recorder", // Name
		PA_STREAM_PLAYBACK, // stream direction
		NULL, // device
		"Recorded playback", // Stream name
		&ss, // Sample spec
		NULL, // Channel map
		NULL, // buffering attributes
		&error
	);
	if (s == NULL) {
		fprintf(stderr, "Error opening for playback: %s", pa_strerror(error));
		exit(1);
	}

	printf("Now playing back\n");
	pa_simple_write(s, data, bytes, NULL);
	pa_simple_drain(s, NULL);
	pa_simple_free(s);
}

int main() {
	size_t bytes = RECORDING_TIME * pa_bytes_per_second(&ss);
	char *data = malloc(bytes);
	record(data, bytes);
	play(data, bytes);
	free(data);
}
