#include <random>
#include "utils.hpp"


float clamp(float x, float lo, float hi)
{
    return x < lo ? lo : x > hi ? hi : x;
}


unsigned char gamma_correction(float x)
{
	return (unsigned char)(pow(clamp(x, 0.0, 1.0), 1 / 2.2) * 255);
}


static std::default_random_engine random_generator;

float uniform_sample(float a, float b)
{
    std::uniform_real_distribution<float> dis(a, b);
    return dis(random_generator);
}