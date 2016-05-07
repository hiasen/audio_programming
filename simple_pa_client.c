/*
 * Simple pulseaudio client.
 *
 * It plays different kinds of wavesforms.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>
#include <string.h>
#include <pulse/simple.h>

#define SAMPLE_RATE 48000

void square(short *data, int num_samples, short amplitude) {
	int i = 0;
	for (; i < num_samples/2; i++) {
		data[i] = amplitude;
	}
	for (; i < num_samples; i++) {
		data[i] = -amplitude;
	}
}

void saw(short *data, int num_samples, short amplitude) {
	short current = 0;
	short increment = 2*amplitude/num_samples;
	int i = 0;
	for (; i < num_samples/2; i++) {
		data[i] = current;
		current += increment;
	}
	current = -amplitude;
	for (; i < num_samples; i++) {
		data[i] = current;
		current += increment;
	}
}

void triangle(short *data, int num_samples, short amplitude) {
	int i = 0;
	short current = 0;
	short increment = 4*amplitude/num_samples;
	for (; i < num_samples/4; i++) {
		data[i] = current;
		current += increment;
	}
	for (; i < 3*num_samples/4; i++) {
		data[i] = current;
		current -= increment;
	}
	for (; i < num_samples; i++) {
		data[i] = current;
		current += increment;
	}
}

struct wave_form {
	char *name;
	void (*writer)(short*, int, short);
};

#define NUM_WAVEFORMS 3
struct wave_form wave_forms[NUM_WAVEFORMS] = {
	{"square", square},
	{"saw", saw},
	{"tri", triangle}
};

struct wave_form get_wave_form(const char *name) {
	for (int i = 0; i < NUM_WAVEFORMS; ++i) {
		if (strcmp(wave_forms[i].name, name) == 0) {
			return wave_forms[i];
		}
	}
	fprintf(stderr, "Unknown waveform %s\n", name);
	exit(1);
}

struct wave_settings {
	int freq;
	int duration;
	struct wave_form wave_form;
};

struct wave_settings config;


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
		case 'w':
			config.wave_form = get_wave_form(arg);
			break;
		default:
			break;
	}
	return 0;
}


struct argp_option my_options[] = {
	{.name="frequency", .key='f', .arg="Frequency in Hz"},
	{.name="duration", .key='d', .arg="Duration in seconds"},
	{.name="type", .key='w', .arg="Wave type (square|saw|tri)"},
};

struct argp my_argp = {.options=my_options, .parser=parser};


void write_repeat(pa_simple *s, short *data, int bytes, int repeats) {
	for (int j = 0; j < repeats; j++){
		pa_simple_write(s, data, bytes, NULL);
	}
}


int main(int argc, char *argv[]){
	config.freq = 440;
	config.duration = 10;
	config.wave_form = wave_forms[0];

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

	int num_samples_in_period = SAMPLE_RATE / config.freq;
	short data[num_samples_in_period];
	short amplitude = (1<<15) - 1;
	config.wave_form.writer(data, num_samples_in_period, amplitude);

	int num_periods = config.freq * config.duration;
	write_repeat(s, data, 2*num_samples_in_period, num_periods);

	pa_simple_free(s);
}
