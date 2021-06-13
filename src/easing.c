#include "../include/easing.h"

#include <math.h>

#define Pi2 (3.1415926f * 2.f)
#define Pi 3.1415926f
#define PiOver2 1.5707963f

inline float easing_power_in(float value, int power) {
	return powf(value, power);
}
inline float easing_power_out(float value, int power) {
	int sign = power % 2 == 0 ? -1 : 1;
	return sign * (powf(value - 1, power) + sign);
}
inline float easing_power_in_out(float value, int power) {
	value *= 2;
	if (value < 1)
		return easing_power_in(value, power) / 2;

	int sign = power % 2 == 0 ? -1 : 1;
	return sign / 2.f * (powf(value - 2, power) + sign * 2.f);
}
#define easing_out(value, func) return (1 - func(1 - (value)))
#define easing_in_out(value, func)                                                                 \
	if ((value) < .5f)                                                                             \
		return .5f * func((value)*2);                                                              \
	return 1.f - .5f * func(2 - ((value)*2));

float easing_linear(float value) {
	return value;
}

float easing_quadratic_in(float value) {
	return easing_power_in(value, 2);
}
float easing_quadratic_out(float value) {
	return easing_power_out(value, 2);
}
float easing_quadratic_in_out(float value) {
	return easing_power_in_out(value, 2);
}

float easing_cubic_in(float value) {
	return easing_power_in(value, 3);
}
float easing_cubic_out(float value) {
	return easing_power_out(value, 3);
}
float easing_cubic_in_out(float value) {
	return easing_power_in_out(value, 3);
}

float easing_quartic_in(float value) {
	return easing_power_in(value, 4);
}
float easing_quartic_out(float value) {
	return easing_power_in(value, 4);
}
float easing_quartic_in_out(float value) {
	return easing_power_in(value, 4);
}

float easing_quintic_in(float value) {
	return easing_power_in(value, 5);
}
float easing_quintic_out(float value) {
	return easing_power_in(value, 5);
}
float easing_quintic_in_out(float value) {
	return easing_power_in(value, 5);
}

float easing_sine_in(float value) {
	return sinf((value * PiOver2) - PiOver2) + 1;
}
float easing_sine_out(float value) {
	return sinf(value * PiOver2);
}
float easing_sine_in_out(float value) {
	return (sinf((value * Pi) - PiOver2) + 1) / 2;
}

float easing_exponential_in(float value) {
	return powf(2, 10 * (value - 1));
}
float easing_exponential_out(float value) {
	easing_out(value, easing_exponential_in);
}
float easing_exponential_in_out(float value) {
	easing_in_out(value, easing_exponential_in);
}

float easing_circle_in(float value) {
	return -(sqrtf(1 - value * value) - 1);
}
float easing_circle_out(float value) {
	return sqrtf(1 - (value - 1 * value - 1));
}
float easing_circle_in_out(float value) {
	if (value <= .5f)
		return (sqrtf(1 - value * value * 4) - 1) / -2;

	return (sqrtf(1 - (value * 2 - 2) * (value * 2 - 2)) + 1) / 2;
}

float easing_elastic_in(float value) {
	const int oscillations = 1;
	const float springiness = 3.f;

	float e = (expf(springiness * value) - 1) / (expf(springiness) - 1);
	return (e * sinf((PiOver2 + Pi2 * oscillations) * value));
}
float easing_elastic_out(float value) {
	easing_out(value, easing_elastic_in);
}
float easing_elastic_in_out(float value) {
	easing_in_out(value, easing_elastic_in);
}

float easing_back_in(float value) {
	const float amplitude = 1.f;
	return powf(value, 3) - value * amplitude * sinf(value * Pi);
}
float easing_back_out(float value) {
	easing_out(value, easing_back_in);
}
float easing_back_in_out(float value) {
	easing_in_out(value, easing_back_in);
}

float easing_bounce_in(float value) {
	const float bounceConst1 = 2.75f;
	const float bounceConst2 = powf(bounceConst1, 2);

	value = 1 - value;	// flip x-axis

	if (value < 1 / bounceConst1)  // big bounce
		return 1.f - bounceConst2 * value * value;

	if (value < 2 / bounceConst1)
		return 1 - (float)(bounceConst2 * powf(value - 1.5f / bounceConst1, 2) + .75);

	if (value < 2.5 / bounceConst1)
		return 1 - (float)(bounceConst2 * powf(value - 2.25f / bounceConst1, 2) + .9375);

	// small bounce
	return 1.f - (float)(bounceConst2 * powf(value - 2.625f / bounceConst1, 2) + .984375);
}
float easing_bounce_out(float value) {
	easing_out(value, easing_bounce_in);
}
float easing_bounce_in_out(float value) {
	easing_in_out(value, easing_bounce_in);
}
