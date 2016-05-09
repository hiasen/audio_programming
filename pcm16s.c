#include "pcm16s.h"
#define PI 3.141592653589323

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
	short increment = (short) (2*amplitude/num_samples);
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
	short increment = (short) (4*amplitude/num_samples);
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


void sine(short *data, int num_samples, short amplitude) {
	for (int i = 0; i < num_samples; ++i) {
		data[i] = (short) ((sin((2*PI*i)/num_samples) * amplitude));
	}
}

#define NUM_WAVEFORMS 4
struct wave_form wave_forms[NUM_WAVEFORMS] = {
	{"square", square},
	{"saw", saw},
	{"tri", triangle},
	{"sine", sine}
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
