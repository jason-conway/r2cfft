/**
 * @file r2cfft.h
 * @author Jason Conway (jpc@jasonconway.dev)
 * @brief Wrapper for the arm_rfft_fast_f32 CMSIS routine
 * @version 0.9
 * @date 2021-08-08
 *
 * @copyright Copyright (c) 2021 Jason Conway. 
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include <WProgram.h>
#include <arm_math.h>
#include <arm_const_structs.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void r2cfft(arm_rfft_fast_instance_f32 *pInstance, float32_t *pIn, float32_t *pOut, size_t fftSize, uint8_t inverseFlag);
#ifdef __cplusplus
};
#endif
