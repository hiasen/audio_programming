/*
 * Functions for writing one period of waveforms in pcm16s format
 */

#ifndef __PCM16S__H
#define  __PCM16S__H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct wave_form {
	char *name;
	void (*writer)(short*, int, short);
};
struct wave_form get_wave_form(const char *name);

void square(short *data, int num_samples, short amplitude);
void saw(short *data, int num_samples, short amplitude);
void triangle(short *data, int num_samples, short amplitude);
void sine(short *data, int num_samples, short amplitude);
#endif
