// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include "fftr.h"
#include <math.h>

// ----------------------------------------------------------------------------
#define FFTR_SHIFT_AMOUNT (16 - FFTR_SIZE_LOG2)

// ----------------------------------------------------------------------------
static fix15_t kAlpha;
static fix15_t hann_window[FFTR_SIZE];
static uint16_t bitrev_lut[FFTR_SIZE];
static fix15_t sine_div2_lut[FFTR_SIZE];

// ----------------------------------------------------------------------------
FFTR_INLINE fix15_t abs_fix15(const fix15_t x)
{
  return ((x + (x >> 31)) ^ (x >> 31));
}

// ----------------------------------------------------------------------------
FFTR_INLINE fix15_t mul_fix15(const fix15_t a, const fix15_t b)
{
  return (fix15_t)(((int64_t)(a) * (int64_t)(b)) >> 15);
}

// ----------------------------------------------------------------------------
static uint16_t bitrev(uint16_t v)
{
  v = ((v & 0x5555u) << 1u) | ((v >> 1u) & 0x5555u);
  v = ((v & 0x3333u) << 2u) | ((v >> 2u) & 0x3333u);
  v = ((v & 0x0f0fu) << 4u) | ((v >> 4u) & 0x0f0fu);
  return ((v >> 8u) & 0x00ffu) | ((v & 0x00ffu) << 8u);
}

// ----------------------------------------------------------------------------
void fftr_init()
{
  kAlpha = float_to_fix15(0.4);
  const float phase_inc = (2 * M_PI) / (float)(FFTR_SIZE);
  for(int32_t i=0;i<FFTR_SIZE;i++)
  {
    bitrev_lut[i] = bitrev(i) >> FFTR_SHIFT_AMOUNT;
    hann_window[i] = float_to_fix15(0.5 * (1.0 - cosf(i * phase_inc)));
    sine_div2_lut[i] = float_to_fix15(0.5 * sinf(i * phase_inc));
  }
}

// ----------------------------------------------------------------------------
void fftr_applyWindow(fix15_t* real)
{
  for(int32_t i=0;i<FFTR_SIZE;i++)
  {
    real[i] = mul_fix15(real[i], hann_window[i]);
  }
}

// ----------------------------------------------------------------------------
void fftr_iterate(fix15_t* real, fix15_t* imag)
{
  // Bit reversal permutation
  for(uint16_t i=1;i<(FFTR_SIZE - 1);i++)
  {
    uint16_t irev = bitrev_lut[i];
    if(irev > i)
    {
      // Don't care about imaginary data since we assume they are all zero.
      fix15_t tmp = real[i];
      real[i] = real[irev];
      real[irev] = tmp;
    }
  }

  // ...
  int32_t L = 1;
  int32_t k = FFTR_SIZE_LOG2 - 1;

  // While the length of the FFT's being combined is less than the number
  // of gathered samples
  while(L < FFTR_SIZE)
  {
    // Determine the length of the FFT which will result from combining
    // two FFT's
    int32_t istep = L << 1;

    // For each element in the FFT's that are being combined
    for(int32_t m=0;m<L;m++)
    {
      // Lookup the trig values for that element
      const int32_t phase = m << k;
      const fix15_t wr =  sine_div2_lut[phase + (FFTR_SIZE / 4)];
      const fix15_t wi = -sine_div2_lut[phase                  ];

      // i gets the index of one of the FFT elements being combined
      for(int32_t i=m;i<FFTR_SIZE;i+=istep)
      {
        // j gets the index of the FFT element being combined with i
        int32_t j = i + L;

        // Compute the trig terms (bottom half of the above matrix)
        fix15_t tr = mul_fix15(wr, real[j]) - mul_fix15(wi, imag[j]);
        fix15_t ti = mul_fix15(wr, imag[j]) + mul_fix15(wi, real[j]);

        // Divide ith index elements by two (top half of above matrix)
        fix15_t qr = real[i] >> 1;
        fix15_t qi = imag[i] >> 1;

        // Compute the new values at each index
        real[j] = qr - tr;
        imag[j] = qi - ti;
        real[i] = qr + tr;
        imag[i] = qi + ti;
      }
    }

    // ...
    k -= 1;
    L = istep;
  }
}

// ----------------------------------------------------------------------------
void fftr_estimateMagnitude(fix15_t* real, fix15_t* imag, fix15_t* magn)
{
  for(int32_t i=0;i<(FFTR_SIZE/2);i++)
  {
    fix15_t real_ = abs_fix15(real[i]);
    fix15_t imag_ = abs_fix15(imag[i]);
    if(real_ > imag_)
    {
      magn[i] = real_ + mul_fix15(kAlpha, imag_);
    }
    else
    {
      magn[i] = imag_ + mul_fix15(kAlpha, real_);
    }
  }
}
