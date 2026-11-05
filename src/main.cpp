#include "SineOscillator.h"
#include "WavHeader.h"
#include "types.h"

#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>

PCM16 getPCM16(float normalized_amplitude)
{
    static const PCM16 max_amplitude =
        static_cast<PCM16>(std::pow(2, 16 - 1) - 1);
    return static_cast<PCM16>(
        std::roundf(normalized_amplitude * max_amplitude));
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

    // Generate the data
    std::vector<PCM16> monoSamples;
    std::vector<PCM16> stereoSamples;

    const int duration = 5; // Seconds
    // This should determine the loudness
    SineOscillator sine_oscillator(777.0f, 0.5f);
    SineOscillator sine_oscillator2(400.0f, 0.5f);
    for (int i = 0; i < SineOscillator::SAMPLE_RATE * duration; ++i)
    {
        stereoSamples.emplace_back(getPCM16(sine_oscillator.getNextSample()));
        stereoSamples.emplace_back(getPCM16(sine_oscillator2.getNextSample()));

        monoSamples.emplace_back(getPCM16(sine_oscillator.getNextSample()));
    }

    // Generate header
    WavHeader wav_header{};
    const u32 data_size = stereoSamples.size() * sizeof(stereoSamples.front());
    const u16 bits_per_sample = 16;
    const u16 channel_number = 2;
    const u32 sample_rate = 44100;
    const u16 block_alignment = bits_per_sample * channel_number / 8;
    const u32 byte_rate = sample_rate * block_alignment;
    const u32 file_size = sizeof(WavHeader) - 8 + data_size;
    wav_header.setHeader(file_size,
                         WavHeader::FMT_CHUNK_SIZE,
                         WavHeader::AUDIO_FORMAT_PCM,
                         channel_number,
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
    audioFile.write(reinterpret_cast<char*>(stereoSamples.data()),
                    stereoSamples.size() * sizeof(PCM16));
    audioFile.close();

    return 0;
}