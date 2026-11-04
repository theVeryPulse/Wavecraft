#include <cmath>
#include <numbers>
#include <fstream>
#include <iostream>

using Sample8Bit = int8_t;
using Sample16Bit = int16_t; // 16-bit: [0, 65535], [-32768, 32767]
using Sample32Bit = float;

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


class WavHeader
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    WavHeader()
    {
    }
    ~WavHeader()
    {
    }
    WavHeader(const WavHeader& other) = delete;
    WavHeader& operator=(const WavHeader& other) = delete;
    //------------------------------- Functions --------------------------------
    //---------------------------------- Data ----------------------------------   

    // RIFF chunk
    struct
    {
        const char riff_marker_[4] = {'R', 'I', 'F', 'F'};
        u_int32_t  file_size_; // Size of the file excluding the 8 bytes metadata
        const char wave_marker_[4] = {'W', 'A', 'V', 'E'};
    };

    // fmt chunk
    struct
    {
        const char fmt_[4] = {'f', 'm', 't', '\0'};

        // PCM uses 16 bytes excluding 8 bytes metadata
        u_int32_t  fmt_chunk_size_;
        
        // 1 = PCM (pulse-code modulation)
        u_int16_t  audio_format_;
        
        // 1 = mono, 2 = stereo
        u_int16_t  channel_number_;
        
        // Hertz, usually 44100 (CD)
        u_int32_t  sample_rate_;
        
        // Bytes per second. Sample rate * bit rate * channels / 8 (44100 * 16 * 1 / 8 = 88200 Byte)
        u_int32_t  byte_rate_;
        
        // Bytes per sample frame. Frame may contain 2 channels, so for 16 bit
        // mono, the block alignment is 2 bytes. But for 16 bit stereo, the 
        // block alignment is 4 bytes (2 bytes each channel for that frame)
        u_int16_t  block_alignment_;
        
        // Bit depth, e.g. 16
        u_int16_t  bits_per_sample_;
    };

    // Data chunk
    struct
    {
        const char data_marker_[4] = {'d', 'a', 't', 'a'};
        u_int32_t  data_size_;
    };
    // char*      data;

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
};

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

    WavHeader wave_header;

    audioFile.write(reinterpret_cast<char*>(&wave_header), sizeof(wave_header));
    
    const int duration = 2;
    Sample16Bit max_amplitude =
        static_cast<Sample16Bit>(std::pow(2, SineOscillator::BIT_DEPTH - 1) - 1);
    for (int i = 0; i < SineOscillator::SAMPLE_RATE * duration; ++i)
    {
        const float normalized_sample = sine_oscillator.getNextSample();
        Sample16Bit sample = static_cast<Sample16Bit>(
            std::roundf(normalized_sample * max_amplitude));
        audioFile.write(reinterpret_cast<char*>(&sample), sizeof(Sample16Bit));
        // audioFile << "\n"; // Real binary doesn't need the newlines
    }

    audioFile.close();
    return 0;
}