#ifndef SRC_EFFECTS_H_
#define SRC_EFFECTS_H_

#include <stdint.h>
#include <math.h>

#define BUFFERSIZE		128
#define SAMPLERATE		44100
#define NUM_ADC_INPUTS	9

#define N 5000
#define halfN 2500
#define ADC_COUNT 3

typedef struct _delay {
	float gain;
	int8_t offsetIndex;
	int8_t currentIndex;
	int16_t delayBuffer[BUFFERSIZE];	// needs to be initialized to all 0s
} Delay;


typedef struct _dynamicRangeCompression {
	float ratio;
	float threshold;
	float attack;
	float release;
} Compression;


typedef struct _equalizer {
	float gain;
	float centerFreq;
	float bandwidth;
} Equalizer;


typedef struct _globals {
	Compression DRC;
	Delay delay;
	Equalizer EQ;
} Global;

// create global structure of structures
Global EffectGlobals;


// effect function prototypes
void processDelay(int16_t *inBufPtr, int16_t *outBufPtr);
void processDRC(int16_t *inBufPtr, int16_t *outBufPtr);
void processEQ(int16_t *inBufPtr, int16_t *outBufPtr);


#endif /* SRC_EFFECTS_H_ */
