#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>

class SineOscillator
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    SineOscillator(float frequency, float amplitude);
    ~SineOscillator();
    SineOscillator(const SineOscillator& other);            // Copy constructor
    SineOscillator& operator=(const SineOscillator& other); // Copy assignment operator
    //------------------------------- Functions --------------------------------
    float process();
    //---------------------------------- Data ----------------------------------
    // Sample rate per second
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
    float amplitude_ = 0;
    float angle_ = 0;
    float offset_ = 0;
};

SineOscillator::SineOscillator(float frequency, float amplitude) : frequency_(frequency), amplitude_(amplitude)
{
    offset_ = 2 * M_PIf * frequency_ / SAMPLE_RATE;
}

SineOscillator::~SineOscillator()
{
}

SineOscillator::SineOscillator(const SineOscillator& other)
{
}

SineOscillator& SineOscillator::operator=(const SineOscillator& other)
{
    return *this;
}

float SineOscillator::process()
{
    const auto sample = amplitude_ * std::sinf(angle_);
    angle_ += offset_;
    return sample;
}

int main()
{
    std::cout << "Hello, world!\n";
    SineOscillator sine_oscillator(440.0f, 0.5f);

    std::ofstream audioFile;
    audioFile.open("waveform", std::ios::binary);
    
    const int duration = 2;

    for (int i = 0; i < SineOscillator::SAMPLE_RATE * duration; ++i)
        audioFile << sine_oscillator.process() << "\n";

    std::cout << "int size: " << sizeof(int) << "\n";
    std::cout << "float size: " << sizeof(float) << "\n";

    // 16-bit: [0, 65535], [-32768, 32767]
    
    audioFile.close();
    return 0;
}