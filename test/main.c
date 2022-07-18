// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "fftr.h"

// ----------------------------------------------------------------------------
float noise()
{
  return (2.0 * (((float)rand() / (float)RAND_MAX) - 0.5));
}

// ----------------------------------------------------------------------------
int main()
{
  fix15_t real[FFTR_SIZE];
  fix15_t imag[FFTR_SIZE];
  fix15_t magn[FFTR_SIZE/2];

  // ...
  fftr_init();

  // ...
  for(int32_t i=0;i<FFTR_SIZE;i++)
  {
    imag[i] = 0;
    real[i] = float_to_fix15(
      1.0 * sinf( 8 * 2 * M_PI * i / (float)FFTR_SIZE) +
      5.0 * cosf(32 * 2 * M_PI * i / (float)FFTR_SIZE) +
      2.0 * noise()
    );
  }

  // ...
  FILE* fp = fopen("input.txt", "w+");
  for(int32_t i=0;i<FFTR_SIZE;i++)
  {
    fprintf(fp, "%7.4f\r\n", fix15_to_float(real[i]));
  }
  fclose(fp);
 
  // ...
  fftr_applyWindow(real);
  fftr_iterate(real, imag);
  fftr_estimateMagnitude(real, imag, magn);

  // ...
  fp = fopen("output.txt", "w+");
  for(int32_t i=0;i<(FFTR_SIZE/2);i++)
  {
    fprintf(fp, "%4d,%7.4f,%7.4f,%7.4f\r\n",
      i,
      fix15_to_float(imag[i]),
      fix15_to_float(real[i]),
      fix15_to_float(magn[i])
    );
  }

  // ...
  fclose(fp);
  return 0;
}
