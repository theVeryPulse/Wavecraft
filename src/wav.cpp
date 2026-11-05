#include <cmath>
#include <numbers>
#include <fstream>
#include <iostream>
#include <vector>

using Sample8Bit = int8_t;
using Sample16Bit = int16_t; // 16-bit: [0, 65535], [-32768, 32767]
using Sample32Bit = float;

using i8 = int8_t;

using u16 = u_int16_t;
using u32 = u_int32_t;
using u64 = u_int64_t;

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
    void setHeader(u32 file_size, u32 fmt_chunk_size, u16 audio_format,
                   u16 channel_number, u32 sample_rate, u32 byte_rate,
                   u16 block_alignment, u16 bits_per_sample, u32 data_size)
    {
        this->file_size_ = file_size;
        this->fmt_chunk_size_ = fmt_chunk_size;
        this->audio_format_ = audio_format;
        this->channel_number_ = channel_number;
        this->sample_rate_ = sample_rate;
        this->byte_rate_ = byte_rate;
        this->block_alignment_ = block_alignment;
        this->bits_per_sample_ = bits_per_sample;
        this->data_size_ = data_size;
    }
    //---------------------------------- Data ----------------------------------
    static constexpr u16 AUDIO_FORMAT_PCM = 1;

    // RIFF chunk
    struct
    {
        const char riff_marker_[4] = {'R', 'I', 'F', 'F'};
        u32 file_size_; // Size of the file excluding the 8 bytes metadata
        const char wave_marker_[4] = {'W', 'A', 'V', 'E'};
    };

    // fmt chunk
    struct
    {
        const char fmt_marker_[4] = {'f', 'm', 't', ' '};

        // PCM uses 16 bytes excluding 8 bytes metadata
        u32 fmt_chunk_size_;

        // 1 = PCM (pulse-code modulation)
        u16 audio_format_;

        // 1 = mono, 2 = stereo
        u16 channel_number_;

        // Hertz, usually 44100 (CD)
        u32 sample_rate_;

        // Bytes per second. Sample rate * bit rate * channels / 8 (44100 * 16 *
        // 1 / 8 = 88200 Byte)
        u32 byte_rate_;

        // Bytes per sample frame. Frame may contain 2 channels, so for 16 bit
        // mono, the block alignment is 2 bytes. But for 16 bit stereo, the
        // block alignment is 4 bytes (2 bytes each channel for that frame)
        u16 block_alignment_;

        // Bit depth, e.g. 16
        u16 bits_per_sample_;
    };

    // Data chunk
    struct
    {
        const char data_marker_[4] = {'d', 'a', 't', 'a'};
        u32        data_size_;
    };

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

    SineOscillator sine_oscillator(200.0f, 0.5f);


    // Generate the data
    std::vector<Sample16Bit> samples;

    const int         duration = 2; // Seconds
    const Sample16Bit max_amplitude = static_cast<Sample16Bit>(
        std::pow(2, SineOscillator::BIT_DEPTH - 1) - 1);
    for (int i = 0; i < SineOscillator::SAMPLE_RATE * duration; ++i)
    {
        const float normalized_sample = sine_oscillator.getNextSample();
        Sample16Bit sample = static_cast<Sample16Bit>(
            std::roundf(normalized_sample * max_amplitude));
        samples.emplace_back(sample);
    }

    // Generate header
    WavHeader wav_header{};
    const u32 data_size = samples.size() * sizeof(samples.front());
    const u16 bits_per_sample = 16;
    const u16 channel_number = 1;
    const u32 sample_rate = 44100;
    const u16 block_alignment = bits_per_sample * channel_number / 8;
    const u32 byte_rate = sample_rate * block_alignment;
    const u32 file_size = sizeof(WavHeader) - 8 + data_size;
    wav_header.setHeader(file_size,
                         16,
                         WavHeader::AUDIO_FORMAT_PCM,
                         1,
                         sample_rate,
                         byte_rate,
                         block_alignment,
                         bits_per_sample,
                         data_size);

    std::ofstream audioFile("waveform.wav", std::ios::binary);

    // Write the header (this should be 44 bytes)
    audioFile.write(reinterpret_cast<char*>(&wav_header), sizeof(wav_header));
    // Write the data at the end
    audioFile.write(reinterpret_cast<char*>(samples.data()),
                    samples.size() * sizeof(Sample16Bit));

    audioFile.close();

    return 0;
}