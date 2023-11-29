#ifndef SRC_EFFECTS_H_
#define SRC_EFFECTS_H_
#include "infrastructure.h"

typedef struct _delay {
	float gain;
	int16_t offsetIndex;
	int16_t currentIndex;
	int16_t delayBuffer[BUFFERSIZE];	// needs to be initialized to all 0s
	int16_t delayTime;
} Delay;


typedef struct _dynamicRangeCompressor {
	float ratio;
	float threshold;
	float attack;
	float release;
} Compressor;


typedef struct _equalizer {
	float gain;
	float centerFreq;
	float bandwidth;
} Equalizer;

typedef struct _chorus {
	float delay;
	float depth;
	float modulation;
	float feedback;
	int16_t offsetIndex;
	int16_t currentIndex;
	int16_t delayBuffer[BUFFERSIZE];	// needs to be initialized to all 0s
} Chorus;

typedef struct _overdrive {
	float drive;
	float tone;
	float level;
} Overdrive;

typedef struct _phaser {
	float stages;
	float depth;
	float modulation;
	float feedback;
} Phaser;


typedef struct _globals {
	Compressor DRC;
	Delay delay;
	Equalizer EQ;
	Chorus chorus;
	Overdrive overdrive;
	Phaser phaser;
} Global;

Global EffectGlobals;	// create global structure of structures


#endif /* SRC_EFFECTS_H_ */
