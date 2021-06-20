#pragma once

#include <stdlib.h>

/**
 * @brief Generates a random integer number from 0 to max (inclusive)
 */
#define RANDN(max) (rand() % (max))
/**
 * @brief Generates a random integer number from min to max (inclusive)
 */
#define RANDR(min, max) ((rand() % ((max) - (min) + 1)) + (min))

/**
 * @brief Generates a random float from 0 to max (inclusive)
 */
#define RANDNF(max) ((float)rand() / (float)(RAND_MAX / max))
/**
 * @brief Generates a random float number from min to max (inclusive)
 */
#define RANDRF(min, max) (((float)rand() / (float)(RAND_MAX / ((max) - (min) + 1))) + (min))
