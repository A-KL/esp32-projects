#pragma once

#include <map>
#include <vector>
#include <Arduino.h>

#include <types.h>

const std::map<const String, const input_type_t> input_type_strings = 
{ 
  { "pwm",  pwm     }, 
  { "adc",  adc     }, 
  { "sbus", sbus    },
  { "esp_now", esp_now },
  { "ps3",   ps3    },
  { "xbox", xbox    }
};

const std::map<const String, const output_type_t> output_type_strings = 
{ 
  { "motor",      dc         }, 
  { "servo",      servo      }, 
  { "servo_lego", servo_lego },
  { "sbus",       sbus_out   },
};

const std::map<const String, const dc_mode_t> drive_mode_string = 
{ 
  { "a_b_en", a_b_en }, 
  { "a_b",    a_b    },
  { "dir_en", dir_en } 
};

template<typename TEnum>
const String* find_by_value(const TEnum value, const std::map<const String, const TEnum> map) 
{
    for (const auto& mapping : map) {
      if (mapping.second == value) {
        return mapping.first;
      }
    }
    return NULL;
  }

template<typename TEnum>
const TEnum to_enum(const String key, const std::map<const String, const TEnum> map) 
{
    auto iter = map.find(key);

    return (iter == map.end() ? (TEnum)-1 : iter->second);
}