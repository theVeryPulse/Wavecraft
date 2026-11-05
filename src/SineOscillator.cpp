#include "SineOscillator.h"
#include <numbers>
#include <cmath>

SineOscillator::SineOscillator(float frequency, float amplitude)
    : frequency_(frequency), amplitude_(amplitude)
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
    const auto sample = amplitude_ * std::sinf(angle_);
    angle_ += offset_;
    return sample;
}