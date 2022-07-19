# fftr
Easy to use™ FFT library for purely real signals, like audio signals, accelerometer / gyro signals etc.

This doesnt claim to be the fastest one but it is not the slowest one either.

Input and output values are in 16.15 fixed point format. 

If you are on a desktop / embedded linux environment, you should probably use <https://www.fftw.org/> but this library can be useful for various MCU based embedded applications.

## Configuration
* Change FFTR_SIZE and corresponding FFTR_SIZE_LOG2 value for your desired FFT size.

## Remember
* If you have 256 point FFT, you will have 128 point magnitude information.
* Conversion formula for output index to frequency is: `F_SAMPLING * (N / (FFT_SIZE / 2)) ` where `N` is the bin number and `F_SAMPLING` is the sampling frequency.
* Zeroth index represent DC component.

## Usage

```C
// Pre allocate neccessary buffers with proper lengths
fix15_t real[FFTR_SIZE];
fix15_t imag[FFTR_SIZE];
fix15_t magn[FFTR_SIZE/2];

// Call this _once_ to fill neccessary lookup tables. We could
// have used "precalculated" lookup table values to save code 
// space if needed.
fftr_init();

// TODO: Fill real array with your signal
// TODO: Fill imag array with all zeros

// Apply hann window to input
fftr_applyWindow(real);

// Do the actual FFT operation
fftr_iterate(real, imag);

// After FFT, you have your imaginary and real parts, but you 
// probably just want to calculate "magnitude" per bin. Following 
// method "estimates" magnitude from imaginary and real portions
// of FFT output. For detailed explanation see the following 
// link: https://dspguru.com/dsp/tricks/magnitude-estimator/
fftr_estimateMagnitude(real, imag, magn);
```

## Reference

This codebase is slightly reformatted and slightly improved version of the algorithm from <https://github.com/Gadgetoid/rp2040-lcd-fft> which is based on <https://vanhunteradams.com/FFT/FFT.html>

### Main Improvements

* Use lookup table for bit reversal permutation. Don't call bitrev method inside `fftr_iterate` call.
* Don't shuffle imaginary input array for bit reversal permutation since we expect to have *purely* real input.
