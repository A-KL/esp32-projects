#pragma once

#include <stddef.h>
#include <stdint.h>
#include <math.h>

struct audio_envelope_context_t 
{
    float envelope_out = 0;
    float envelope_in = 0;
    float envelope_level = 0;
    float attack = 0.996879878;
    float release = 0.9996879878;
    float dc_filter = 0;
    float dc_filter_alpha = 0.99f;
    float x_prev = 0;   
    float needle_velocity = 0;
    float sample_period = 1.0f / 44100.0f;
    float needle_stiffness = 355.30576f;
    float needle_damping = 33.9292f;
};

template<typename T>
struct audio_sample_t {
    T right;
    T left;
};

inline void envelope_init(audio_envelope_context_t& context, const uint16_t sample_rate)
{
    const float rate = sample_rate > 0 ? static_cast<float>(sample_rate) : 44100.0f;
    constexpr float needle_frequency = 3.0f;
    constexpr float damping_ratio = 0.9f;
    constexpr float two_pi = 6.28318531f;
    const float omega = two_pi * needle_frequency;

    context.sample_period = 1.0f / rate;
    context.needle_stiffness = omega * omega;
    context.needle_damping = 2.0f * damping_ratio * omega;
    context.attack = expf(-1.0f / (rate * 0.010f));
    context.release = expf(-1.0f / (rate * 0.050f));
    context.dc_filter_alpha = expf(-2.0f * 3.14159265f * 20.0f / rate);
    context.envelope_out = 0;
    context.envelope_in = 0;
    context.envelope_level = 0;
    context.dc_filter = 0;
    context.x_prev = 0;
    context.needle_velocity = 0;
}

// template<typename T>
// inline void map_sample(const uint8_t* samples, const uint8_t bps, const uint16_t sample_index, const audio_sample_t<T>** result)
// {
//     *result = (const audio_sample_t<T>*)(samples + sample_index * bps * 2);
// }

template<typename T>
inline const audio_sample_t<T>* get_sample(const uint8_t* samples, const size_t sample_index)
{
    return reinterpret_cast<const audio_sample_t<T>*>(samples + sample_index * sizeof(T) * 2);
}

bool get_sample_f(const uint8_t* samples, const size_t index, const uint8_t bits_per_sample, float& left, float& right)
{
    switch (bits_per_sample)
    {
    case 8: {
        const auto* sample = get_sample<int8_t>(samples, index);
        left = sample->left;
        right = sample->right;
        return true;
    }
    case 16: {
        const auto* sample = get_sample<int16_t>(samples, index);
        left = sample->left;
        right = sample->right;
        return true;
    }
    case 24: {
    case 32: {
        const auto* sample = get_sample<int32_t>(samples, index);
        left = sample->left;
        right = sample->right;
        return true; 
    }
    }
    default:
        break;
    }
    return false;
}

inline void envelope_process_sample(audio_envelope_context_t& context, const float input)
{
    const float filtered = input - context.x_prev + context.dc_filter_alpha * context.dc_filter;
    context.x_prev = input;
    context.dc_filter = filtered;
    context.envelope_in = fabsf(filtered);

    const float difference = context.envelope_level - context.envelope_in;
    const float smoothing = context.envelope_in > context.envelope_level
        ? context.attack
        : context.release;
    context.envelope_level = context.envelope_in + smoothing * difference;

    const float acceleration =
        context.needle_stiffness * (context.envelope_level - context.envelope_out)
        - context.needle_damping * context.needle_velocity;

    context.needle_velocity += acceleration * context.sample_period;
    context.envelope_out += context.needle_velocity * context.sample_period;

    if (context.envelope_out <= 0.0f) {
        context.envelope_out = 0.0f;
        context.needle_velocity = 0.0f;
    }
}

template<typename T>
void envelope_calculate_right_left(const uint8_t* samples, const float gain, const size_t samples_count, audio_envelope_context_t& right, audio_envelope_context_t& left)
{
    for (auto i = 0; i < samples_count; i++)
    {
        const auto* sample = get_sample<T>(samples, i);
        envelope_process_sample(left, static_cast<float>(sample->left) * gain);
        envelope_process_sample(right, static_cast<float>(sample->right) * gain);
    }
}

void envelope_calculate_right_left(const uint8_t* samples, const float gain, const size_t samples_count, const uint8_t bpp, audio_envelope_context_t& right, audio_envelope_context_t& left)
{
    float sample_left = 0;
    float sample_right = 0;

    for (auto i = 0; i < samples_count; i++)
    {
        get_sample_f(samples, i, bpp, sample_left, sample_right);
        envelope_process_sample(left, sample_left * gain);
        envelope_process_sample(right, sample_right * gain);
    }
}

inline void envelope_calculate(const int16_t* samples, const float gain, const int16_t samples_count, audio_envelope_context_t& context)
{
    for (auto i = 0; i<samples_count; i++) 
    {
        envelope_process_sample(context, static_cast<float>(samples[i]) * 10.0f);
    }
}