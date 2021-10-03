/**
 * @file r2cfft.c
 * @author Jason Conway (jpc@jasonconway.dev)
 * @brief Wrapper for the arm_rfft_fast_f32 CMSIS routine
 * @version 0.9
 * @date 2021-08-08
 *
 * @copyright Copyright (c) 2021 Jason Conway. All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

#include "r2cfft.h"
#pragma GCC optimize("O3")

/**
 * @brief In-place complex number swapping routine
 * 
 * @param cmplx Array of complex values formatted cmplx[2*n] = {Re[0], Im[0], Re[1], Im[2], ... Re[n], Im[n]}
 * @param size Number of complex values in cmplx, [n]
 */
void cmplxSwap(float32_t *cmplx, size_t size)
{
	//   [0]   [1]   [2]   [3] ...  [2n-3]  [2n-2][2n-1]  [2n]
	// Re[0] Im[0] Re[1] Im[1] ... Re[n-1] Im[n-1] Re[n] Im[n]
	for (size_t i = 0; i < size / 2; i += 2)
	{
		// Re = cmplx[0], cmplx[2], cmplx[4], ... cmplx[n/2 - 2]
		// Im = cmplx[1], cmplx[3], cmplx[4], ... cmplx[n/2 - 1]
		float32_t real = cmplx[i];			
		float32_t imaginary = cmplx[i + 1]; 

		// cmplx[0] <= cmplx[n-4], cmplx[2] <= cmplx[n-6], ... cmplx[n/2 - 2] <= cmplx[n/2 - 2]
		// cmplx[1] <= cmplx[n-3], cmplx[3] <= cmplx[n-5], ... cmplx[n/2 - 1] <= cmplx[n/2 - 1]
		cmplx[i] = cmplx[size - i - 2];		
		cmplx[i + 1] = cmplx[size - i - 1]; 

		// cmplx[n-4] <= cmplx[0], cmplx[n-6] <= cmplx[2] ... cmplx[n/2 - 2] <= cmplx[n/2 - 2]
		// cmplx[n-3] <= cmplx[1], cmplx[n-5] <= cmplx[3] ... cmplx[n/2 - 1] <= cmplx[n/2 - 1]
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
 * @param fftLength Size of the RFFT
 * @param inverseFlag Flag for forward / inverse transform
 */
void r2cfft(arm_rfft_fast_instance_f32 *pInstance, float32_t *pIn, float32_t *pOut, size_t fftLength, uint8_t inverseFlag)
{
	if (inverseFlag)
	{
		// iRFFT expects Re[n/2] packed into Im[0]
		pIn[1] = pIn[fftLength];
	}
	arm_rfft_fast_f32(pInstance, pIn, pOut, inverseFlag);
	if (inverseFlag)
	{
		return;
	}

	arm_cmplx_conj_f32(&pOut[2], &pOut[fftLength + 2], (fftLength / 2) - 1); // Complex conjugate first half into second half
	cmplxSwap(&pOut[fftLength + 2], (fftLength - 2));						 // Flip frequencies of the second half
	pOut[fftLength] = pOut[1];												 // pOut[1] = Re[n/2]
	pOut[1] = 0;															 // Im[0] always 0 (Re[0] is DC)
}
