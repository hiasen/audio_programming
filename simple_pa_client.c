/*
 * Simple pulseaudio client.
 *
 * It plays a square wave.
 */

#include <stdlib.h>
#include <argp.h>
#include <pulse/simple.h>

#define SAMPLE_RATE 44100

struct wave_settings {
	int freq;
	int duration;
};

struct wave_settings config = {
	.freq = 220,
	.duration = 5
};


pa_sample_spec ss = {
	.format = PA_SAMPLE_S16NE,
	.channels = 1,
	.rate = SAMPLE_RATE
};

error_t parser(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'f':
			config.freq = strtol(arg, NULL, 10);
			break;
		case 'd':
			config.duration = strtol(arg, NULL, 10);
			break;
		default:
			break;
	}
	return 0;
}


struct argp_option my_options[2] = {
	{.name="frequency", .key='f', .arg="Frequency in Hz"},
	{.name="duration", .key='d', .arg="Duration in seconds"},
};

struct argp my_argp = {.options=my_options, .parser=parser};


void square(short *data, int num_samples, short amplitude) {
	int i = 0;
	for (; i < num_samples/2; i++) {
		data[i] = amplitude;
	}
	for (; i < num_samples; i++) {
		data[i] = -amplitude;
	}
}


void write_repeat(pa_simple *s, short *data, int bytes, int repeats) {
	for (int j = 0; j < repeats; j++){
		pa_simple_write(
			s,
			data,
			bytes,
			NULL
		);
	}
}


int main(int argc, char *argv[]){
	error_t err_argp = argp_parse(&my_argp, argc, argv, 0, NULL, 0);
	if (err_argp) {
		fprintf(stderr, "Error in argp_parse.\n");
		exit(1);
	}

	pa_simple *s = pa_simple_new(
		NULL,
		"Simple pulseaudio client",
		PA_STREAM_PLAYBACK,
		NULL,
		"Rectangular wave",
		&ss,
		NULL,
		NULL,
		NULL
	);

	int num_samples_in_period = SAMPLE_RATE / config.freq;
	short data[num_samples_in_period];
	short amplitude = (1<<15) - 1;
	square(data, num_samples_in_period, amplitude);

	int num_periods = config.freq * config.duration;
	write_repeat(s, data, 2*num_samples_in_period, num_periods);

	pa_simple_free(s);
}
