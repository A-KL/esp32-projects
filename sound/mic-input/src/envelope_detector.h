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