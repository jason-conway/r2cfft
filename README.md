# r2cfft

A minimal R2C FFT wrapper for the CMSIS DSP Library

## Description

r2cfft implements a wrapper for the arm_rfft_fast_f32() function, producing an identifically formated output as the arm_cfft_f32() function. It is intended to provide a high-efficiency near drop-in-replacement for arm_cfft_f32() when used on real-valued sequences.

The arm_rfft_fast_f32() function performs the FFT of a real sequence of length N, and returns an output array of length N due to the even symmetry present in the frequency domain. The r2cfft wrapper performs the real N-point FFT using arm_rfft_fast_f32(), placing the N-point result in a buffer of size 2N. The complex conjugate of points [0] through [N-1] is taken and placed between points [N] through [2 * N - 1]. The frequencies in points [N] through [2 * N - 1] are then swapped to yield an a complete 2N-sized output equivalent to the cfft function. 

## Performance

The table below shows elapsed clock cycles for 128, 256, 512, and 1024 point FFTs on a Teensy 4.X

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

#### Note

Like with arm_rfft_fast_f32(), pIn will be modified by the operation. 

### Dependencies

* CMSIS DSP Library

### Installing

* Add r2cfft.c and r2cfft.h from /lib/r2cfft to your project. 

## Version History

* 0.9
    * Initial Release

## License

r2cfft is licensed under the [MIT] License - see LICENSE for details