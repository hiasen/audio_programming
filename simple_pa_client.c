/*
 * Simple pulseaudio client.
 *
 * It plays a square wave.
 */

#include <pulse/simple.h>

#define SAMPLE_RATE 44100
#define FREQ 220
#define DURATION 2


pa_sample_spec ss = {
	.format = PA_SAMPLE_S16NE,
	.channels = 1,
	.rate = SAMPLE_RATE
};


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


int main(){
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

	int num_samples_in_period = SAMPLE_RATE / FREQ;
	short data[num_samples_in_period];
	short amplitude = (1<<15) - 1;
	square(data, num_samples_in_period, amplitude);

	int num_periods = FREQ * DURATION;
	write_repeat(s, data, 2*num_samples_in_period, num_periods);

	pa_simple_free(s);
}
