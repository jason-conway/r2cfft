/**
 * @file r2cfft.c
 * @author Jason Conway (jpc@jasonconway.dev)
 * @brief Wrapper for the arm_rfft_fast_f32 CMSIS routine
 * @version 0.9
 * @date 2021-08-08
 *
 * @copyright Copyright (c) 2021 Jason Conway
 * SPDX-License-Identifier: MIT
 */

#include "r2cfft.h"

/**
 * @brief In-place complex number swap
 * 
 * @param cmplx Array of complex values
 * @param size Number of complex values in cmplx
 */
void cmplxSwap(float32_t *cmplx, size_t size)
{
	//   [0]   [1]   [2]   [3] ...  [2n-3]  [2n-2][2n-1]  [2n]
	// Re[0] Im[0] Re[1] Im[1] ... Re[n-1] Im[n-1] Re[n] Im[n]
	for (size_t i = 0; i < size / 2; i += 2)
	{
		float32_t real = cmplx[i];			// [0] [2] [4] [6] ... [2n]
		float32_t imaginary = cmplx[i + 1]; // [1] [3] [5] [7] ... [2n-1]

		cmplx[i] = cmplx[size - i - 2];		// [0] = [2n-1], [2] = [2n-3]
		cmplx[i + 1] = cmplx[size - i - 1]; // [1] = [2n], [3] = [2n-2]

		cmplx[size - i - 2] = real;
		cmplx[size - i - 1] = imaginary;
	}
}

/**
 * @brief Format the output of arm_rfft_fast_f32 to match that of arm_cfft_f32
 * 
 * @param pInstance Pointer to an arm_rfft_fast_instance_f32 instance structure
 * @param pIn Pointer to the input buffer; input is modified during the operation
 * @param pOut Pointer to the output buffer
 * @param fftSize Size of the RFFT
 * @param inverseFlag Select forward (0) or inverse (1) RFFT
 */
void r2cfft(arm_rfft_fast_instance_f32 *pInstance, float32_t *pIn, float32_t *pOut, size_t fftSize, uint8_t inverseFlag)
{
	if (inverseFlag)
	{
		// iRFFT expects Re[n/2] packed into Im[0]
		pIn[1] = pIn[fftSize];
	}
	arm_rfft_fast_f32(pInstance, pIn, pOut, inverseFlag);
	if (inverseFlag)
	{
		return;
	}

	arm_cmplx_conj_f32(&pOut[2], &pOut[fftSize + 2], (fftSize / 2) - 1); // Complex conjugate first half into the second half
	cmplxSwap(&pOut[fftSize + 2], (fftSize - 2)); // Flip frequencies of the second half
	pOut[fftSize] = pOut[1]; // pOut[1] = Re[n/2]
	pOut[1] = 0; // Im[0] always 0
}
