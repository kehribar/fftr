// ----------------------------------------------------------------------------
// 
// 
// ----------------------------------------------------------------------------
// 
// Based on: 
//   https://github.com/Gadgetoid/rp2040-lcd-fft
// 
// Which is based on: 
//   https://vanhunteradams.com/FFT/FFT.html 
// 
// ----------------------------------------------------------------------------
#ifndef FFTR_H
#define FFTR_H

// ----------------------------------------------------------------------------
#include <stdint.h>

// ----------------------------------------------------------------------------
#define FFTR_SIZE (512)
#define FFTR_SIZE_LOG2 (9)

// ----------------------------------------------------------------------------
#define FFTR_INLINE __attribute__((always_inline)) static inline

// ----------------------------------------------------------------------------
typedef int32_t fix15_t;

// ----------------------------------------------------------------------------
FFTR_INLINE fix15_t float_to_fix15(float a)
{
  return (fix15_t)(a * 32767.0f);
}

// ----------------------------------------------------------------------------
FFTR_INLINE float fix15_to_float(fix15_t a)
{
  return (float)(a) / 32767.0f;
}

// ----------------------------------------------------------------------------
void fftr_init();

// ----------------------------------------------------------------------------
void fftr_applyWindow(fix15_t* real);

// ----------------------------------------------------------------------------
void fftr_iterate(fix15_t* real, fix15_t* imag);

// ----------------------------------------------------------------------------
void fftr_estimateMagnitude(fix15_t* real, fix15_t* imag, fix15_t* magn);

// ----------------------------------------------------------------------------
#endif
