#pragma once
#ifndef SINE_OSCILLATOR_H
#define SINE_OSCILLATOR_H

#include "types.h"

class SineOscillator
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------

    /**
     * @brief Construct a new Sine Oscillator object
     *
     * @param frequency
     * @param max_normalized_amplitude [0, 1]
     */
    SineOscillator(float frequency, float max_normalized_amplitude);
    ~SineOscillator();
    SineOscillator(const SineOscillator& other) = delete;
    SineOscillator& operator=(const SineOscillator& other) = delete;
    //------------------------------- Functions --------------------------------
    float getNextSample();
    //---------------------------------- Data ----------------------------------
    // Sample rate per second. 44100 is slightly greater than twice the human
    // hearing range maximum.
    static constexpr float SAMPLE_RATE = 44100;

  protected:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    //------------------------------- Functions --------------------------------
    //---------------------------------- Data ----------------------------------

  private:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    //------------------------------- Functions --------------------------------
    //---------------------------------- Data ----------------------------------
    float frequency_ = 0;
    float max_normalized_amplitude_ = 0;
    float angle_ = 0;
    float offset_ = 0;
};

#endif /* SINE_OSCILLATOR_H */
