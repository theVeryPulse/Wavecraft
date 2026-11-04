#include <numbers>
#include <fstream>
#include <iostream>

using Sample8Bit = int8_t;
using Sample16Bit = int16_t;
using Sample32Bit = float;

class SineOscillator
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    SineOscillator(float frequency, float amplitude);
    ~SineOscillator();
    SineOscillator(const SineOscillator& other); // Copy constructor
    SineOscillator& operator=(
        const SineOscillator& other); // Copy assignment operator
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

SineOscillator::SineOscillator(float frequency, float amplitude)
    : frequency_(frequency), amplitude_(amplitude)
{
    offset_ = 2 * std::numbers::pi_v<float> * frequency_ / SAMPLE_RATE;
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

float SineOscillator::getNextSample()
{
    const auto sample = amplitude_ * std::sinf(angle_);
    angle_ += offset_;
    return sample;
}

int main()
{
#ifdef __linux__
    std::cout << "Running on Linux\n";
#elif defined(__APPLE__)
    std::cout << "Running on macOS\n";
#elif defined(_WIN32)
    std::cout << "Running on Windows\n";
#else
    std::cout << "Unknown platform\n";
#endif

    SineOscillator sine_oscillator(440.0f, 0.5f);

    std::ofstream audioFile("waveform", std::ios::binary);

    const int duration = 2;

    Sample16Bit max_amplitude =
        static_cast<Sample16Bit>(pow(2, SineOscillator::BIT_DEPTH - 1) - 1);
    for (int i = 0; i < SineOscillator::SAMPLE_RATE * duration; ++i)
    {
        const float normalized_sample = sine_oscillator.getNextSample();
        Sample16Bit sample = static_cast<Sample16Bit>(
            std::roundf(normalized_sample * max_amplitude));
        audioFile.write(reinterpret_cast<char*>(&sample), sizeof(Sample16Bit));
        // audioFile << "\n"; // Real binary doesn't need the newlines
    }

    std::cout << "int size: " << sizeof(int) << "\n";
    std::cout << "float size: " << sizeof(float) << "\n";

    // 16-bit: [0, 65535], [-32768, 32767]

    audioFile.close();
    return 0;
}