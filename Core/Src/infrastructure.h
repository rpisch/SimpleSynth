#ifndef SRC_INFRASTRUCTURE_H_
#define SRC_INFRASTRUCTURE_H_


#include <stdint.h>
#include <math.h>

#define BUFFERSIZE		2048
#define SAMPLERATE		44100
#define NUM_ADC_INPUTS	10

// function prototypes
extern void initializeGlobals();

// effect function prototypes
extern int16_t applyDelay(int16_t signal);
extern int16_t applyDRC(int16_t signal);
extern int16_t applyEQ(int16_t signal);
extern int16_t applyChorus(int16_t signal);
extern int16_t applyOverdrive(int16_t signal);
extern int16_t applyPhaser(int16_t signal);

#endif /* SRC_INFRASTRUCTURE_H_ */
