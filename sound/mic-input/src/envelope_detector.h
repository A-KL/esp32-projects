#pragma once

#include <math.h>

struct audio_envelope_context_t 
{
    float envelope_out = 0;
    float envelope_in = 0;
    float attack = 0.996879878;
    float release = 0.9996879878;
    float dc_filter = 0;
    float x_prev = 0;   
};

void envelope_init(audio_envelope_context_t& context, const uint16_t sample_rate) 
{
    context.attack = expf(-1.0/((float)sample_rate * .050)); //50mS Attack
    context.release = expf(-1.0/((float)sample_rate * .100)); //100mS Release
}

void envelope_calculate_right_left(const int32_t* samples, const int16_t samples_count, audio_envelope_context_t& right, audio_envelope_context_t& left)
{
    float left_x = 0;
    float right_x = 0;

    for (auto i=0; i<samples_count; i++)
    {
        left_x = (int16_t) samples[i];
        right_x = (int16_t) (samples[i] >> 16);

        left_x = left_x * 10;
        right_x = right_x * 10;

        left.dc_filter = left_x - left.x_prev + (.99 * left.dc_filter);
        left.x_prev = left_x;

        right.dc_filter = right_x - right.x_prev + (.99 * right.dc_filter);
        right.x_prev = right_x;

        left.envelope_in = fabs(left.dc_filter);
        right.envelope_in = fabs(right.dc_filter);

        auto diff_left = left.envelope_out - left.envelope_in;
        auto diff_right = right.envelope_out - right.envelope_in;

        if (left.envelope_out < left.envelope_in) 
        {
            left.envelope_out = left.envelope_in + left.attack * diff_left;
        }
        else 
        {
            left.envelope_out = left.envelope_in + left.release * diff_left;
        }

        if (right.envelope_out < right.envelope_in) 
        {
            right.envelope_out = right.envelope_in + right.attack * diff_right;
        }
        else 
        {
            right.envelope_out = right.envelope_in + right.release * diff_right;
        }
    }
}

void envelope_calculate(const int16_t* samples, const int16_t samples_count, audio_envelope_context_t& context)
{
    float x = 0;

    for (auto i=0; i<samples_count; i++) 
    {
        x = samples[i];
        x = x * 10;

        context.dc_filter = x - context.x_prev + (.99 * context.dc_filter);
        context.x_prev = x;

        context.envelope_in = fabs(context.dc_filter);
        

        auto diff = context.envelope_out - context.envelope_in;

        if (context.envelope_out < context.envelope_in) 
        {
            context.envelope_out = context.envelope_in + context.attack * diff;
        }
        else 
        {
            context.envelope_out = context.envelope_in + context.release * diff;
        }
    }
}