#include "SineOscillator.h"
#include "WavHeader.h"
#include "types.h"

#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>

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

    // Generate the data
    std::vector<Sample16Bit> samples;

    const int         duration = 2; // Seconds
    const Sample16Bit max_amplitude = static_cast<Sample16Bit>(
        std::pow(2, SineOscillator::BIT_DEPTH - 1) - 1);
    SineOscillator sine_oscillator(200.0f, 0.5f);
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

    // Create wav file
    std::ofstream audioFile("waveform.wav", std::ios::binary);
    // Write the header (this should be 44 bytes)
    audioFile.write(reinterpret_cast<char*>(&wav_header), sizeof(wav_header));
    // Write the data at the end
    audioFile.write(reinterpret_cast<char*>(samples.data()),
                    samples.size() * sizeof(Sample16Bit));
    audioFile.close();

    return 0;
}