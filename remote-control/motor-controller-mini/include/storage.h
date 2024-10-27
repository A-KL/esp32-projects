#pragma once

#include <iostream>
#include <string>
#include <ArduinoJson.h>

#include "FS.h"
#include <LittleFS.h>

inline void storage_init() 
{
  if (!LittleFS.begin(true)) {
    log_e("[STORAGE] An error has occurred while mounting LittleFS");
  } else {
    log_i("[STORAGE] LittleFS mounted successfully");
  }
}

String setting_read_key(const String& key, const char* fileName = "/default.json")
{
    File file = LittleFS.open(fileName, FILE_READ);

    if (!file)
    {
        log_e("[STORAGE] There was an error opening '%s' file", "/default_v2.json");
        file.close();
        return "";
    }
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {       
        doc.clear();
        file.close();
        return "";
    }

    auto root = doc.as<JsonObject>();
    auto result = root[key].as<String>();

    doc.clear();
    file.close();

    return result;
}

bool settings_load(global_config_t& config, const char* file_name = "/default_v2.json")
{
    File file = LittleFS.open(file_name, FILE_READ);
    if (!file) {
        log_e("[STORAGE] There was an error opening %s file", file_name);
        file.close();
        return false;
    }
    
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        log_e("[STORAGE] There was an error deserializing json from %s", file_name);
        doc.clear();
        file.close();
        return false;
    }

    auto root = doc.as<JsonObject>();

    for (JsonPair kv : root) {
        auto& input_configs = config[kv.key().c_str()];
        auto values = kv.value().as<JsonArray>();

        log_i("[STORAGE] Found %d configuration(s) for %s:", values.size(), kv.key().c_str());

        for (auto value : values) {
            input_config_t config;
            config.in_channel = value["in_ch"].as<short>();
            config.out_channel = value["out_ch"].as<short>();
            config.out_type = string_to_output_type(value["out_type"].as<String>());
            input_configs.push_back(config);

            log_d("[STORAGE] %d (in_ch:%d\tout_type:%d\tout_ch:%d)", kv.key().c_str(), config.in_channel, config.out_type, config.out_channel);
        }
    }

    doc.clear();
    file.close();

    log_i("Configuration loading...\tDONE");

    return true;
}

// bool settings_load(motor_config_t motors[], const int count)
// {
//     File file = LittleFS.open("/default.json", FILE_READ);
//     if (!file)
//     {
//         Serial.println("There was an error opening default.json file");
//         file.close();
//         return false;
//     }
    
//     StaticJsonDocument<512> doc;
//     DeserializationError error = deserializeJson(doc, file);

//     if (error) {       
//         doc.clear();
//         file.close();
//         return false;
//     }

//     auto root = doc.as<JsonObject>();
//     auto motors_json = root["motors"].as<JsonArray>();

//     if (count > motors_json.size())
//     {
//         Serial.print("Configuration doesn't cover all motors: ");
//         Serial.println(motors_json.size());
//         file.close();
//         return false;
//     }

//     auto i = 0;

//     for (JsonVariant motor_json : motors_json) 
//     {
//         motors[i].mode = motor_json["mode"].as<motor_mode_t>();
//         motors[i].inverted = motor_json["inv"].as<bool>();
//         motors[i].input_channel =  motor_json["ch"].as<int>();

//         auto input_type_str = motor_json["type"].as<String>();
//         auto input_type_iter = drive_input_map.find(input_type_str);

//         if (input_type_iter == drive_input_map.end())
//         {
//             Serial.print("Unable to map value: ");
//             Serial.println(input_type_str);
//             //log_e("Netif Get IP Failed!");
//             file.close();
//             return false;
//         }

//         motors[i].input_type = input_type_iter->second;

//         i++;
//     }

//     file.close();
//     return true;
// }

void settings_save(const char* fileName, uint8_t *data, size_t len, size_t index, size_t total)
{
    File file = LittleFS.open(fileName, "w+");
    file.write(data, total);
    file.close();
}

bool settings_apply(uint8_t *data, size_t len, size_t index, size_t total)
{
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, (const char *)data, total);

    if (error) {       
        doc.clear();
        return false;
    }

    auto root = doc.as<JsonObject>();
    auto motors_json = root["motors"].as<JsonArray>();

    for (auto motor_json : motors_json) {
        log_d("[Storage] Apply(mode:%d inverted:%d input_type:%s input_channel:%d",
            motor_json["mode"].as<int>(),
            motor_json["inverted"].as<bool>(),
            motor_json["input_type"].as<String>(),
            motor_json["input_channel"].as<int>());
    }

    return true;
}