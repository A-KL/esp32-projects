#pragma once

#include <iostream>
#include <string>
#include <ArduinoJson.h>

#include "FS.h"
#include <LittleFS.h>

#ifndef DEVICE_ID
#error "Device-ID is not defined!"
#endif

#define Q(x) #x
#define QUOTE(x) Q(x)

#define CONCAT2(a, b) /a.b
#define CONCAT(a, b) CONCAT2(a, b)

#ifndef DEVICE_CONFIG_FILE
// #define DEVICE_CONFIG_FILE_FORMAT(x) QUOTE("/config_ ## x ## .json")
// #define DEVICE_CONFIG_FILE DEVICE_CONFIG_FILE_FORMAT(qwewq)
#define DEVICE_CONFIG_FILE QUOTE(CONCAT(DEVICE_ID, json)) 
// "/default_v2.json"
#endif

#include <serialize.h>

void storage_init() 
{
  if (!LittleFS.begin(true)) {
    log_e("An error has occurred while mounting LittleFS");
  } else {
    log_i("LittleFS mounted successfully");
  }
}

String setting_read_key(const String& key, const char* fileName)
{
    File file = LittleFS.open(fileName, FILE_READ);

    if (!file)
    {
        log_e("[STORAGE] There was an error opening '%s' file", fileName);
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

bool settings_load(global_config_t& config, const char* file_name = DEVICE_CONFIG_FILE)
{
    log_i("Loading json configuration from %s", file_name);

    File file = LittleFS.open(file_name, FILE_READ);
    if (!file) {
        log_e("[Error] There was an error opening %s file", file_name);
        file.close();
        return false;
    }
    
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        log_e("[Error] There was an error deserializing json from %s", file_name);
        doc.clear();
        file.close();
        return false;
    }

    auto root = doc.as<JsonObject>();

    for (JsonPair kv : root) {

        auto key = to_enum<input_type_t>(kv.key().c_str(), input_type_strings);
        auto& input_configs = config[key];
        auto values = kv.value().as<JsonArray>();

        log_i("Found %d configuration(s) for %s:", values.size(), kv.key().c_str());

        for (const auto& value : values) {
            input_config_t config;
            config.in_channel = value["in_ch"].as<short>();
            config.out_channel = value["out_ch"].as<short>();
            config.out_type = to_enum<output_type_t>(value["out_type"].as<String>(), output_type_strings);
            input_configs.push_back(config);

            log_d(" - %s (in_ch:%d\tout_type:%d\tout_ch:%d)", kv.key().c_str(), config.in_channel, config.out_type, config.out_channel);
        }
    }

    doc.clear();
    file.close();

    log_i("Configuration loaded!");

    return true;
}

void settings_map_inputs(global_config_t& configs, const input_type_t input_type, const int16_t* inputs, const output_type_t output_type, int16_t* outputs, const uint8_t inputs_count)
{
    if (inputs_count == 0) {
        return;
    }
    for (const auto& config : configs[input_type]) {
        if (config.out_type == output_type) {
            outputs[config.out_channel] = inputs[config.in_channel];
        }
    }
}

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