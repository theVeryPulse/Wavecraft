#pragma once
#ifndef WAV_HEADER_H
#define WAV_HEADER_H

#include "types.h"

class WavHeader
{
  public:
    //--------------- Types: Classes, Enums, and Aliases (using) ---------------
    //----------------- Constructors, Assignments, Destructor ------------------
    WavHeader();
    ~WavHeader();
    WavHeader(const WavHeader& other) = delete;
    WavHeader& operator=(const WavHeader& other) = delete;
    //------------------------------- Functions --------------------------------
    void setHeader(u32 file_size, u32 fmt_chunk_size, u16 audio_format,
                   u16 channel_number, u32 sample_rate, u32 byte_rate,
                   u16 block_alignment, u16 bits_per_sample, u32 data_size);
    //---------------------------------- Data ----------------------------------
    static constexpr u16 AUDIO_FORMAT_PCM = 1;
    static constexpr u32 FMT_CHUNK_SIZE = 16;

    // The Order and the data type of the members must not be changed. The are
    // arranged according to 
    // RIFF chunk
    //==========================================================================
    const char riff_marker_[4] = {'R', 'I', 'F', 'F'};
    u32 file_size_; // Size of the file excluding the 8 bytes metadata
    const char wave_marker_[4] = {'W', 'A', 'V', 'E'};
    // fmt chunk
    //==========================================================================
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
    // Data chunk
    //==========================================================================
    const char data_marker_[4] = {'d', 'a', 't', 'a'};
    u32        data_size_;

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

#endif /* WAV_HEADER_H */