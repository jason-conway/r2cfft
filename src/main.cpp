#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <WProgram.h>

#include "usb_serial.h"
#include "imxrt.h"

#include "r2cfft.h"
#include "realSignal.h"

#define FFT_SIZE 1024

usb_serial_class *stdSerial = &Serial;
arm_rfft_fast_instance_f32 rfftInstance;

int main(void)
{
	stdSerial->begin(115200);

	while (!(stdSerial))
	{
		delay(1000);
	}

	char clrSeq[] = {0x1B, 0x5B, 0x32, 0x4A, 0x00};
	stdSerial->printf("%s", clrSeq);

	arm_rfft_fast_init_f32(&rfftInstance, FFT_SIZE);

	float32_t rfftIn[FFT_SIZE];
	float32_t rfftOut[2 * FFT_SIZE];

	for (size_t i = 0; i < FFT_SIZE / 2; i++)
	{
		rfftIn[i + FFT_SIZE / 2] = realSignal[i];
	}

	uint32_t rfftStart = ARM_DWT_CYCCNT;

	r2cfft(&rfftInstance, rfftIn, rfftOut, FFT_SIZE, Forward);

	uint32_t rfftStop = ARM_DWT_CYCCNT;

	r2cfft(&rfftInstance, rfftOut, rfftIn, FFT_SIZE, Inverse);

	uint32_t irfftStop = ARM_DWT_CYCCNT;

	stdSerial->printf("==Length %u==\r\nRFFT: %u\r\niRFFT: %u\r\n", FFT_SIZE, rfftStop - rfftStart, irfftStop - rfftStop);

	exit(EXIT_SUCCESS);
}
