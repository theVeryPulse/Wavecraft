#pragma once
#ifndef SINE_OSCILLATOR_H
#define SINE_OSCILLATOR_H

#include "types.h"

class SineOscillator
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    SineOscillator(float frequency, float amplitude);
    ~SineOscillator();
    SineOscillator(const SineOscillator& other) = delete;
    SineOscillator& operator=(const SineOscillator& other) = delete;
    //------------------------------- Functions --------------------------------
    float getNextSample();
    //---------------------------------- Data ----------------------------------
    // Sample rate per second. 44100 is slightly greater than twice the human
    // hearing range maximum.
    static constexpr float SAMPLE_RATE = 44100;
    static constexpr int   BIT_DEPTH = sizeof(Sample16Bit) * 8;

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
    float amplitude_ = 0;
    float angle_ = 0;
    float offset_ = 0;
};

#endif /* SINE_OSCILLATOR_H */
