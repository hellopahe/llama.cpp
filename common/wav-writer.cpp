#include "wav-writer.h"
#include <algorithm>

bool wav_writer::write(const std::vector<float> & audio_data, int sample_rate, const std::string & output_path) {
    if (audio_data.empty()) {
        return false;
    }

    std::ofstream file(output_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    wav_header header;
    header.sample_rate = sample_rate;
    header.byte_rate = sample_rate * header.num_channels * (header.bits_per_sample / 8);
    header.block_align = header.num_channels * (header.bits_per_sample / 8);

    // Convert float audio data to 16-bit PCM
    std::vector<int16_t> pcm_data;
    pcm_data.reserve(audio_data.size());

    for (float sample : audio_data) {
        // Clamp to [-1, 1] range
        float clamped = std::max(-1.0f, std::min(1.0f, sample));
        // Convert to 16-bit integer
        int16_t pcm_sample = static_cast<int16_t>(clamped * 32767.0f);
        pcm_data.push_back(pcm_sample);
    }

    // Update header sizes
    header.data_size = pcm_data.size() * sizeof(int16_t);
    header.file_size = 36 + header.data_size; // 36 = header size without RIFF chunk size field

    // Write header
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Write audio data
    file.write(reinterpret_cast<const char*>(pcm_data.data()), header.data_size);

    file.close();
    return true;
}
