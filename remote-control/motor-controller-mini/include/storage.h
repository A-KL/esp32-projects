#pragma once

#include <iostream>
#include <string>
#include <ArduinoJson.h>

#include "FS.h"
#include <LittleFS.h>

#include <types.h>

motor_config_t motors_config[motors_count];

global_config_t global_config;

void storage_init() 
{
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

String setting_read_key(const String& key)
{
    File file = LittleFS.open("/default.json", FILE_READ);

    if (!file)
    {
        Serial.println("There was an error opening default.json file");
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

bool settings_load_v2(global_config_t& config)
{
    File file = LittleFS.open("/default_v2.json", FILE_READ);
    if (!file)
    {
        Serial.println("There was an error opening default_v2.json file");
        file.close();
        return false;
    }
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {       
        doc.clear();
        file.close();
        return false;
    }

    auto root = doc.as<JsonObject>();

    for (JsonPair kv : root) {
        Serial.println(kv.key().c_str());
        Serial.println(kv.value().as<JsonArray>().size());
    }

    // auto motors_json = root["motors"].as<JsonArray>();

    // if (count > motors_json.size())
    // {
    //     Serial.print("Configuration doesn't cover all motors: ");
    //     Serial.println(motors_json.size());
    //     file.close();
    //     return false;
    // }

    // auto i = 0;

    // for (JsonVariant motor_json : motors_json) 
    // {
    //     motors[i].mode = motor_json["mode"].as<motor_drive_mode_t>();
    //     motors[i].inverted = motor_json["inv"].as<bool>();
    //     motors[i].input_channel =  motor_json["ch"].as<int>();

    //     auto input_type_str = motor_json["type"].as<String>();
    //     auto input_type_iter = drive_input_map.find(input_type_str);

    //     if (input_type_iter == drive_input_map.end())
    //     {
    //         Serial.print("Unable to map value: ");
    //         Serial.println(input_type_str);
    //         //log_e("Netif Get IP Failed!");
    //         file.close();
    //         return false;
    //     }

    //     motors[i].input_type = input_type_iter->second;

    //     i++;
    // }

    file.close();
    return true;
}

bool settings_load(motor_config_t motors[], const int count)
{
    File file = LittleFS.open("/default.json", FILE_READ);
    if (!file)
    {
        Serial.println("There was an error opening default.json file");
        file.close();
        return false;
    }
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {       
        doc.clear();
        file.close();
        return false;
    }

    auto root = doc.as<JsonObject>();
    auto motors_json = root["motors"].as<JsonArray>();

    if (count > motors_json.size())
    {
        Serial.print("Configuration doesn't cover all motors: ");
        Serial.println(motors_json.size());
        file.close();
        return false;
    }

    auto i = 0;

    for (JsonVariant motor_json : motors_json) 
    {
        motors[i].mode = motor_json["mode"].as<motor_drive_mode_t>();
        motors[i].inverted = motor_json["inv"].as<bool>();
        motors[i].input_channel =  motor_json["ch"].as<int>();

        auto input_type_str = motor_json["type"].as<String>();
        auto input_type_iter = drive_input_map.find(input_type_str);

        if (input_type_iter == drive_input_map.end())
        {
            Serial.print("Unable to map value: ");
            Serial.println(input_type_str);
            //log_e("Netif Get IP Failed!");
            file.close();
            return false;
        }

        motors[i].input_type = input_type_iter->second;

        i++;
    }

    file.close();
    return true;
}

void settings_save(uint8_t *data, size_t len, size_t index, size_t total)
{
    File file = LittleFS.open("/default.json", "w+");
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

    for (JsonVariant motor_json : motors_json) {
        Serial.println(motor_json["mode"].as<int>());
        Serial.println(motor_json["inverted"].as<bool>());
        Serial.println(motor_json["input_type"].as<String>());
        Serial.println(motor_json["input_channel"].as<int>());
    }

    return true;
}