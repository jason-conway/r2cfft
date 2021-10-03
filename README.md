# r2cfft

A minimal R2C FFT wrapper for the CMSIS DSP Library

## Description

r2cfft implements a wrapper for the arm_rfft_fast_f32() function, producing an identically formatted output as the arm_cfft_f32() function. It is intended to provide a high-efficiency near drop-in-replacement for arm_cfft_f32() when used on real-valued sequences.

The arm_rfft_fast_f32() function performs the FFT of a real sequence of length N and returns an output array of length N due to the even symmetry present in the frequency domain. The r2cfft wrapper performs the real N-point FFT using arm_rfft_fast_f32(), placing the N-point result in a buffer of size 2N. The complex conjugate of points [0] through [N-1] is taken and placed between points [N] through [2 * N - 1]. The frequencies in points [N] through [2 * N - 1] are then swapped to yield an a complete 2N-sized output equivalent to the cfft function. 

## Performance

The table below shows elapsed clock cycles for 128-, 256-, 512-, and 1024-point FFTs on a Teensy 4.X

| Size | r2cfft | Inverse r2cfft | arm_cfft_f32 | Inverse arm_cfft_f32 |
|------|--------|----------------|--------------|----------------------|
| 128  | 4262   | 4283           | 5865         | 7019                 |
| 256  | 9648   | 9893           | 13015        | 15628                |
| 512  | 20374  | 20943          | 25229        | 29198                | 
| 1024 | 40015  | 39966          | 60462        | 69584                |

## Getting Started

### Usage

Include the r2cfft header:
```c
#include <r2cfft.h>
```

Define the size of the FFT:
```c
#define FFT_SIZE 1024
```

Initialize an RFFT/IRFFT instance structure:
```c
arm_rfft_fast_instance_f32 rfftInstance; 
```

Initialize the instance structure from within a function body.
```c
arm_rfft_fast_init_f32(&rfftInstance, FFT_SIZE);
```

The forward and inverse R2C FFTs can then be performed using
```c
r2cfft(&rfftInstance, pIn, pOut, FFT_SIZE, Forward);
```
and
```c
r2cfft(&rfftInstance, pIn, pOut, FFT_SIZE, Inverse);
```

See main.cpp for example usage.

#### Note

* Unlike arm_cfft_f32(), the arm_rfft_fast_f32() function does not compute the RFFT / IRFFT in-place. Depending on your application, this could eliminate the need for a memcpy(), but for other applications, an additional buffer is needed- potentially requiring twice the RAM as with the in-place CFFT.

For instance, the process of initializing an FDL containing the full spectra of a high-tap-count minimum-phase FIR filter could be converted from:
```c
arm_cfft_f32(&arm_cfft_sR_f32_len256, partitionBuffer, Forward, 1);
arm_copy_f32(partitionBuffer, longFIR[i], 512);
```
to 
```c
r2cfft(&rfftInstance, partitionBuffer, longFIR[i], 512, Forward);
```

* Like with arm_rfft_fast_f32(), the input buffer is modified by the function.

### Dependencies

* CMSIS DSP Library

### Installing

* Add /lib/r2cfft to your project's /lib folder. 
* TODO: Arduino compatibility? 

## Version History

* 0.9
    * Initial Release

## References

* [CMSIS RFFT](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__RealFFT.html#ga5d2ec62f3e35575eba467d09ddcd98b5)
* [CMSIS CFFT](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__ComplexFFT.html#gade0f9c4ff157b6b9c72a1eafd86ebf80)

## License

r2cfft is licensed under the MIT License - see LICENSE for details