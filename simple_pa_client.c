/*
 * Simple pulseaudio client.
 *
 * It plays different kinds of wavesforms on a single channel.
 */

#include <stdbool.h>
#include <argp.h>
#include <string.h>
#include <pulse/simple.h>

#include "pcm16s.h"
#define SAMPLE_RATE 48000

pa_sample_spec ss = {
	.format = PA_SAMPLE_S16NE,
	.channels = 1,
	.rate = SAMPLE_RATE
};

int freq;
int duration;
struct wave_form wave_form;

error_t parser(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'f':
			freq = strtol(arg, NULL, 10);
			break;
		case 'd':
			duration = strtol(arg, NULL, 10);
			break;
		case 'w':
			wave_form = get_wave_form(arg);
			break;
		default:
			break;
	}
	return 0;
}


struct argp_option my_options[] = {
	{.name="frequency", .key='f', .arg="Frequency in Hz"},
	{.name="duration", .key='d', .arg="Duration in seconds"},
	{.name="type", .key='w', .arg="Wave type (sine|square|saw|tri)"},
};

struct argp my_argp = {.options=my_options, .parser=parser};


void write_repeat(pa_simple *s, short *data, int bytes, int repeats) {
	for (int j = 0; j < repeats; j++){
		pa_simple_write(s, data, bytes, NULL);
	}
}


int main(int argc, char *argv[]){
	// Setting default values
	freq = 440;
	duration = 10;
	wave_form = get_wave_form("sine");

	if (argp_parse(&my_argp, argc, argv, 0, NULL, 0)) {
		fprintf(stderr, "Error in argp_parse.\n");
		exit(1);
	}

	pa_simple *s = pa_simple_new(
		NULL,
		"Simple pulseaudio client",
		PA_STREAM_PLAYBACK,
		NULL,
		"Some waveform",
		&ss,
		NULL,
		NULL,
		NULL
	);

	int num_samples_in_period = SAMPLE_RATE / freq;
	short data[num_samples_in_period];
	short amplitude = (1<<15) - 1;
	wave_form.writer(data, num_samples_in_period, amplitude);

	int num_periods = freq * duration;
	write_repeat(s, data, 2*num_samples_in_period, num_periods);

	pa_simple_free(s);
}
