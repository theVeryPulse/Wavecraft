#include "WavHeader.h"

WavHeader::WavHeader()
{
}

WavHeader::~WavHeader()
{
}

void WavHeader::setHeader(u32 file_size, u32 fmt_chunk_size, u16 audio_format,
                          u16 channel_number, u32 sample_rate, u32 byte_rate,
                          u16 block_alignment, u16 bits_per_sample,
                          u32 data_size)
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