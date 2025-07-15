#include "audio_processor.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>
#include <alsa/asoundlib.h>
#include <cstring>

static const char* TAG = "audio_processor";

// WAV file header structure
struct WAVHeader {
    char riff[4];                    // "RIFF"
    unsigned int size;               // 4 + (8 + size of subchunks)
    char wave[4];                    // "WAVE"
    char fmt[4];                     // "fmt "
    unsigned int fmt_size;           // 16 for PCM
    unsigned short audio_format;     // 1 for PCM
    unsigned short num_channels;     // 1 or 2
    unsigned int sample_rate;        // 16000
    unsigned int byte_rate;          // sample_rate * num_channels * bytes_per_sample
    unsigned short block_align;      // num_channels * bytes_per_sample
    unsigned short bits_per_sample;  // 16
    char data[4];                    // "data"
    unsigned int data_size;          // num_samples * num_channels * bytes_per_sample
};

// WAV file writing function
void write_wav_header(std::ofstream& file, unsigned int data_size) {
    WAVHeader header;
    std::memset(&header, 0, sizeof(header));

    std::strcpy(header.riff, "RIFF");
    header.size = 36 + data_size;
    std::strcpy(header.wave, "WAVE");
    std::strcpy(header.fmt, "fmt ");
    header.fmt_size        = 16;
    header.audio_format    = 1;  // PCM
    header.num_channels    = CHANNELS;
    header.sample_rate     = SAMPLE_RATE;
    header.byte_rate       = SAMPLE_RATE * CHANNELS * 2;  // 16-bit (2 bytes)
    header.block_align     = CHANNELS * 2;
    header.bits_per_sample = 16;
    std::strcpy(header.data, "data");
    header.data_size = data_size;

    file.write(reinterpret_cast<char*>(&header), sizeof(header));
}

// Memory monitoring function
size_t getCurrentRSS() {
    std::ifstream stat_file("/proc/self/statm");
    std::string dummy;
    size_t rss_pages;
    
    if (stat_file >> dummy >> rss_pages) {
        return rss_pages * 4; // Convert pages to KB (assuming 4KB pages)
    }
    return 0;
}

ma::Model* initialize_audio_model(const std::string& model_path) noexcept {
    // For now, return nullptr and handle gracefully
    MA_LOGI(TAG, "Audio model initialization not implemented yet");
    return nullptr;
}

std::vector<float> capture_audio(int duration_seconds) noexcept {
    std::vector<float> audio_data;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* params;
    int err;
    unsigned int rate = SAMPLE_RATE;
    
    // Create a local copy to avoid corruption
    int recording_duration = duration_seconds;
    
    MA_LOGI(TAG, "capture_audio called with duration_seconds = %d", duration_seconds);
    MA_LOGI(TAG, "local copy recording_duration = %d", recording_duration);
    
    // Ensure minimum duration
    if (recording_duration <= 0) {
        MA_LOGW(TAG, "Invalid duration %d, setting to 3 seconds", recording_duration);
        recording_duration = 3;
    }
    
    MA_LOGI(TAG, "Opening audio device hw:0,0");
    
    // Open PCM device for recording
    err = snd_pcm_open(&handle, "hw:0,0", SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot open audio device: %s", snd_strerror(err));
        return audio_data;
    }

    // Allocate hardware parameters
    snd_pcm_hw_params_alloca(&params);
    
    // Fill params with default values
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot initialize hw params: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set access mode
    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set access: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set format
    err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set format: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set channels
    err = snd_pcm_hw_params_set_channels(handle, params, CHANNELS);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set channels: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set sample rate
    err = snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set rate: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set buffer time
    unsigned int buffer_time = 100000; // 100ms
    err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, 0);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set buffer time: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Set period time
    unsigned int period_time = buffer_time / 4;
    err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, 0);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot set period time: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Apply parameters
    err = snd_pcm_hw_params(handle, params);
    if (err < 0) {
        MA_LOGE(TAG, "Cannot apply hw params: %s", snd_strerror(err));
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Calculate total samples needed using working example approach
    snd_pcm_uframes_t chunk_size, buffer_size;
    snd_pcm_hw_params_get_period_size(params, &chunk_size, 0);
    snd_pcm_hw_params_get_buffer_size(params, &buffer_size);
    
    // Calculate buffer requirements like working example
    size_t bits_per_sample = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16_LE);
    size_t bits_per_frame = bits_per_sample * CHANNELS;
    size_t chunk_bytes = chunk_size * bits_per_frame / 8;
    u_char* audiobuf = (u_char*)malloc(chunk_bytes);
    
    if (audiobuf == NULL) {
        MA_LOGE(TAG, "Not enough memory for audio buffer");
        snd_pcm_close(handle);
        return audio_data;
    }
    
    MA_LOGI(TAG, "Audio parameters set - Rate: %u, Chunk size: %lu, Chunk bytes: %zu", rate, chunk_size, chunk_bytes);
    
    // Calculate frames and iterations like working example
    snd_pcm_uframes_t frames = chunk_size;  // Use chunk_size directly like working example
    MA_LOGI(TAG, "Before calculation: recording_duration = %d", recording_duration);
    int total_iterations = (SAMPLE_RATE * recording_duration) / frames;  // Use local copy
    
    MA_LOGI(TAG, "Recording %d seconds - frames per read: %lu, total iterations: %d", recording_duration, frames, total_iterations);
    MA_LOGI(TAG, "Debug calculation: (%d * %d) / %lu = %d", SAMPLE_RATE, recording_duration, frames, total_iterations);
    
    // Create WAV file
    std::string filename = "/home/recamera/audio_capture_" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) + ".wav";
    std::ofstream wav_file(filename, std::ios::binary);
    
    if (!wav_file.is_open()) {
        MA_LOGE(TAG, "Cannot create WAV file: %s", filename.c_str());
        free(audiobuf);
        snd_pcm_close(handle);
        return audio_data;
    }
    
    // Write WAV header
    write_wav_header(wav_file, recording_duration * SAMPLE_RATE * CHANNELS * 2);
    
    // Start recording using working example approach
    auto start_time = std::chrono::steady_clock::now();
    
    for (int i = 0; i < total_iterations; ++i) {
        int pcm = snd_pcm_readi(handle, audiobuf, frames);
        
        if (pcm < 0) {
            if (pcm == -EPIPE) {
                MA_LOGW(TAG, "Audio overrun, recovering...");
                snd_pcm_prepare(handle);
                continue;
            } else {
                MA_LOGE(TAG, "Read error: %s", snd_strerror(pcm));
                break;
            }
        }
        
        // Write to WAV file
        wav_file.write(reinterpret_cast<char*>(audiobuf), pcm * 2);
        
        // Convert to float for return value
        int16_t* samples = reinterpret_cast<int16_t*>(audiobuf);
        for (int j = 0; j < pcm; ++j) {
            float sample_float = static_cast<float>(samples[j]) / 32768.0f;
            audio_data.push_back(sample_float);
        }
    }
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    wav_file.close();
    free(audiobuf);
    snd_pcm_close(handle);
    
    MA_LOGI(TAG, "Captured %zu samples in %ld ms, saved to %s", audio_data.size(), duration_ms.count(), filename.c_str());
    
    return audio_data;
}

std::string process_audio_whisper(ma::Model*& model, const std::vector<float>& audio_data) {
    if (!model) {
        return "No model available - captured " + std::to_string(audio_data.size()) + " audio samples";
    }
    
    // TODO: Implement actual Whisper processing when model is available
    return "Whisper processing not implemented yet - captured " + std::to_string(audio_data.size()) + " samples";
}

void release_audio_model(ma::Model*& model) {
    if (model) {
        // TODO: Implement proper model cleanup
        model = nullptr;
    }
}
