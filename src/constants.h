#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SAMPLE_RATE       16000
#define FFT_SIZE          1024
#define INPUT_BUFFER_SIZE 256
#define INPUT_SHIFT       2

#define FFT_BINS_SKIP     5
#define FFT_MAG_MAX       2000.0

#define MAGNITUDE_HISTORY_SIZE 10 * SAMPLE_RATE / INPUT_BUFFER_SIZE
// 10 sec * 16000 samples per sec / 64 samples for every step forward

#define BPM_MIN           80
#define BPM_MAX           200

#endif //CONSTANTS_H