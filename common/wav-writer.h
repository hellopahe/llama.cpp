#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

// Simple WAV file writer for audio output
class wav_writer {
private:
    struct wav_header {
        char riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t file_size = 0;
        char wave[4] = {'W', 'A', 'V', 'E'};
        char fmt[4] = {'f', 'm', 't', ' '};
        uint32_t fmt_size = 16;
        uint16_t audio_format = 1; // PCM
        uint16_t num_channels = 1;
        uint32_t sample_rate = 16000;
        uint32_t byte_rate = 0;
        uint16_t block_align = 0;
        uint16_t bits_per_sample = 16;
        char data[4] = {'d', 'a', 't', 'a'};
        uint32_t data_size = 0;
    };

public:
    // Write audio data to WAV file
    // audio_data: PCM audio samples (float32, range -1.0 to 1.0)
    // sample_rate: sampling rate in Hz
    // output_path: path to output WAV file
    static bool write(const std::vector<float> & audio_data, int sample_rate, const std::string & output_path);
};
