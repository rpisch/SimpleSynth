#ifndef SRC_EFFECTS_C
#define SRC_EFFECTS_C
#include "effects.h"
#include "infrastructure.h"

// just initialize the globals
void initializeGlobals() {

	//Global EffectGlobals;

	EffectGlobals.delay.gain = 0.5f;
	EffectGlobals.delay.offsetIndex = BUFFERSIZE/2;
	EffectGlobals.delay.currentIndex = 0;
	for (int i = 0; i < BUFFERSIZE; i++)
		EffectGlobals.delay.delayBuffer[i] = 0;

	EffectGlobals.DRC.ratio = 0.0f;
	EffectGlobals.DRC.threshold = 0.0f;
	EffectGlobals.DRC.attack = 0.0f;
	EffectGlobals.DRC.release = 0.0f;

	EffectGlobals.EQ.gain = 0.0f;
	EffectGlobals.EQ.centerFreq = 0.0f;
	EffectGlobals.EQ.bandwidth = 0.0f;

	EffectGlobals.chorus.currentIndex = 0;
	EffectGlobals.chorus.offsetIndex = 0;
	EffectGlobals.chorus.delay = 0.;
	EffectGlobals.chorus.depth = 0.;
	EffectGlobals.chorus.modulation = 0.;
	EffectGlobals.chorus.feedback = 0.;
	for (int i = 0; i < BUFFERSIZE; i++)
		EffectGlobals.chorus.delayBuffer[i] = 0;

	EffectGlobals.overdrive.drive = 0.;
	EffectGlobals.overdrive.tone = 0.;
	EffectGlobals.overdrive.level = 0.;
}


// DELAY
int16_t applyDelay(int16_t signal) {

	int16_t delaySamples = (int16_t) EffectGlobals.delay.delayTime * SAMPLERATE / 1000;
	EffectGlobals.delay.offsetIndex = (EffectGlobals.delay.currentIndex - delaySamples + SAMPLERATE) % SAMPLERATE;

	int16_t delayedSample = EffectGlobals.delay.delayBuffer[EffectGlobals.delay.offsetIndex-1];

	EffectGlobals.delay.delayBuffer[EffectGlobals.delay.currentIndex] = signal + EffectGlobals.delay.gain * delayedSample;

	EffectGlobals.delay.currentIndex = (EffectGlobals.delay.currentIndex+1) % SAMPLERATE;

	return signal + EffectGlobals.delay.gain * delayedSample;

}



// DYNAMIC RANGE COMPRESSION
int16_t applyDRC(int16_t signal) {

	static double envelope = 1.0;
	double loudness, attackAlpha, releaseAlpha;
	double reducedGain, gain = 0.0;

	if (signal != 0.0) {
		// get volume in dB
		loudness = 20.0 * log10(fabs(signal));

		// attack and release time constants
		attackAlpha = exp(-1.0 / (EffectGlobals.DRC.attack * 0.001 * SAMPLERATE));
		releaseAlpha = exp(-1.0 / (EffectGlobals.DRC.release * 0.001 * SAMPLERATE));

		// update envelope
		envelope = (loudness > envelope) ? (attackAlpha * envelope + (1-attackAlpha) * loudness) : (releaseAlpha * envelope + (1-releaseAlpha) * loudness);

		// determine gain reduction based on threshold and gain
		if (envelope > EffectGlobals.DRC.threshold)
			reducedGain = (EffectGlobals.DRC.threshold - envelope) * (1.0 - 1.0 / EffectGlobals.DRC.ratio);

		// determine gain
		gain = pow(10, reducedGain / 20.0);

		// apply gain
		signal = gain * signal;
	}

	return signal;
}





//////////////////// THIS DOES NOT WORK //////////////////////////////////////
// EQUALIZER
int16_t applyEQ(int16_t signal) {

	/*
	// angular frequency
	double omega = 2.0 * M_PI * EffectGlobals.EQ.centerFreq / SAMPLERATE;

	// quality factor
	double	Q = EffectGlobals.EQ.centerFreq / EffectGlobals.EQ.bandwidth;

	// filter coefficients
	double alpha = sin(omega) / (Q * 2.0);
	double beta = 2.0 * sqrt(Q) * alpha;

	double y;

	// apply algorithm
	for (int i = 0; i < BUFFERSIZE; i++) {

		y = (alpha * inBufPtr[i]) + (alpha * beta * inBufPtr[i]);
		y = y - (beta * inBufPtr[i]) - (alpha * beta * y);
		y *= EffectGlobals.EQ.gain;

		// send equalized sample into output buffer
		outBufPtr[i] = y;
	}
	*/
}
////////////////////////////////////////////////////////////////////////////





int16_t applyChorus(int16_t signal) {

	double modulation;
	int delaySamples;
	double delayedSample;

	// calculate the delay time based on modulation
	modulation = EffectGlobals.chorus.depth * sin(2 * M_PI * EffectGlobals.chorus.modulation * EffectGlobals.chorus.offsetIndex / 1000);
	delaySamples = (int)(EffectGlobals.chorus.delay * (1.0 + EffectGlobals.chorus.modulation) * SAMPLERATE / 1000.0);

	// read delayed sample
	EffectGlobals.chorus.currentIndex = (EffectGlobals.chorus.offsetIndex - delaySamples + SAMPLERATE) % SAMPLERATE;
	delayedSample = delayBuffer[readIndex];

	// update delay buffer
	EffectGlobals.chorus.delayBuffer[EffectGlobals.chorus.offsetIndex] = signal + EffectGlobals.chorus.feedback * delayedSample;
	EffectGlobals.chorus.offsetIndex = (EffectGlobals.chorus.offsetIndex + 1) % SAMPLERATE;

	// output
	return signal + EffectGlobals.chorus.feedback * delayedSample;

}





int16_t applyOverdrive(int16_t signal) {

	float distorted = signal * EffectGlobals.overdrive.drive;

	if (distorted > 1.0)
		distorted = 1.0;
	if (distorted < -1.0)
		distorted = -1.0;

	distorted = distorted * (1.0 - EffectGlobals.overdrive.tone) + tanh(EffectGlobals.overdrive.tone * distorted) / tanh(EffectGlobals.overdrive.tone);

	return distorted * EffectGlobals.overdrive.level;


}

#endif


