#pragma once

#include <sscma.h>
#include <string>
#include <vector>

// Audio processing functions
ma::Model* initialize_audio_model(const std::string& model_path) noexcept;
std::vector<float> capture_audio(int duration_seconds = 5) noexcept;
std::string process_audio_whisper(ma::Model*& model, const std::vector<float>& audio_data);
void release_audio_model(ma::Model*& model);

// Audio configuration
constexpr int SAMPLE_RATE = 16000;
constexpr int CHANNELS = 1;
constexpr int BITS_PER_SAMPLE = 16;
