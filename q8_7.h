/*
 * q8_7.c
 *
 *  Created on: 6 Mar 2018
 *      Author: ahmed.zamouche@gmail.com
 */

#ifndef INC_FFT_Q8_7_H_
#define INC_FFT_Q8_7_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int16_t Q8_7_t;

#define Q8_7_MAX INT16_MAX
#define Q8_7_MIN INT16_MIN

#define Q8_7_SHIFT	(7)

typedef struct Complex_s{
	Q8_7_t re;
	Q8_7_t im;
}Complex_t;

typedef struct Polar_s{
	Q8_7_t amp;
	Q8_7_t phase;
}Polar_t;

__attribute__((always_inline))
static inline Q8_7_t _Add(Q8_7_t a, Q8_7_t b) {
	Q8_7_t c;
	int32_t temp;

	temp = (int32_t) a + b;

	if (temp > Q8_7_MAX)		/*saturate the result before assignment*/
		c = Q8_7_MAX;
	else if (temp < Q8_7_MIN)
		c = Q8_7_MIN;
	else
		c = temp;
	return c;
}

__attribute__((always_inline))
static inline Q8_7_t _Sub(Q8_7_t a, Q8_7_t b) {
	Q8_7_t c;
	int32_t temp;

	temp = (int32_t) a - b;

	if (temp > Q8_7_MAX)		/*saturate the result before assignment*/
		c = Q8_7_MAX;
	else if (temp < Q8_7_MIN)
		c = Q8_7_MIN;
	else
		c = temp;
	return c;
}

__attribute__((always_inline))
static inline Q8_7_t _Mul(Q8_7_t a, Q8_7_t b) {
	Q8_7_t c;
	int32_t temp;

	temp = (int32_t) a * b; 	/*Cast operands to 16 bits and multiply*/
	temp +=  (1 << (Q8_7_SHIFT-1));	 	/*add 1/2 to give correct rounding*/
	temp >>= Q8_7_SHIFT;				 	/*divide by 2^7*/
	if (temp > Q8_7_MAX)		/*saturate the result before assignment*/
		c = Q8_7_MAX;
	else if (temp < Q8_7_MIN)
		c = Q8_7_MIN;
	else
		c = temp;
	return c;
}

__attribute__((always_inline))
static inline Q8_7_t _Div(Q8_7_t a, Q8_7_t b) {
	Q8_7_t c;
	int32_t temp;
	temp = (int32_t) a << Q8_7_SHIFT; 	/* cast operand to 32 bits and shift*/
	temp += b >> 1; 			/*Add b/2 to give correct rounding*/
	temp /= b;					/*Perform the division (expensive!)*/
	c = temp;					/*Truncate and assign result*/
	return c;
}

#ifdef __cplusplus
}
#endif

#endif /* INC_FFT_Q8_7_H_ */
