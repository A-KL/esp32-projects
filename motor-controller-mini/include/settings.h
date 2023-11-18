#ifndef settings_h
#define settings_h

#include <Preferences.h>
#include <config.h>

#include <iostream>
#include <string>

const char* settings_namespace = "settings";

Preferences preferences;

void settings_load(controller_config& config)
{
    preferences.begin(settings_namespace, true);

    for (int i=0; i < sizeof(config.motors); ++i) 
    {
        auto index = std::to_string(i);

        auto prefix = ("motor:" + index + ":");

        config.motors[i].mode = (motor_drive_mode_t)preferences.getUInt((prefix + "mode").c_str(), motor_drive_mode_t::a_b_en);
        config.motors[i].inverted = preferences.getBool((prefix + "inverted").c_str(), false);
        config.motors[i].input_type = (input_type_t)preferences.getUInt((prefix + "input_type").c_str(), input_type_t::pwm);
        config.motors[i].input_channel = preferences.getUInt((prefix + "input_channel").c_str(), 0);
    } 

    preferences.end();
}

void settings_save(const controller_config& config)
{
    preferences.begin(settings_namespace, false);

    for (int i=0; i < sizeof(config.motors); ++i) 
    {
        auto index = std::to_string(i);

        auto prefix = ("motor:" + index + ":");

        auto result = preferences.putUInt((prefix + "mode").c_str(), config.motors[i].mode);
        result = preferences.putBool((prefix + "inverted").c_str(), config.motors[i].inverted);
        result = preferences.putUInt((prefix + "input_type").c_str(), config.motors[i].input_type);
        result = preferences.putUInt((prefix + "input_channel").c_str(), config.motors[i].input_channel);
    } 

    preferences.end();

     // ESP.restart();
}

#endif