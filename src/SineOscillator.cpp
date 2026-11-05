#include "SineOscillator.h"
#include <numbers>
#include <cmath>

SineOscillator::SineOscillator(float frequency, float max_normalized_amplitude)
    : frequency_(frequency), max_normalized_amplitude_(max_normalized_amplitude)
{
    offset_ = 2 * std::numbers::pi_v<float> * frequency_ / SAMPLE_RATE;
}

SineOscillator::~SineOscillator()
{
}

#if 0
SineOscillator::SineOscillator(const SineOscillator& other)
{
}

SineOscillator& SineOscillator::operator=(const SineOscillator& other)
{
    return *this;
}
#endif

float SineOscillator::getNextSample()
{
    const auto sample = max_normalized_amplitude_ * std::sinf(angle_);
    angle_ += offset_;
    return sample;
}