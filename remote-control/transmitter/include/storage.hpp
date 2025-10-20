#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <esp32-hal-log.h>

const String Empty("");

void storage_init() 
{
  if (!LittleFS.begin(true)) {
    log_i("An error has occurred while mounting storage");
  }
  log_i("Storage mounted successfully");
}

String setting_read(const String& key, const char* filePath = "/default.json")
{
    File file = LittleFS.open(filePath, FILE_READ);

    if (!file)
    {
        log_e("There was an error opening %s file", filePath);
        return Empty;
    }
    
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {       
        doc.clear();
        file.close();
        return Empty;
    }

    auto root = doc.as<JsonObject>();
    auto result = root[key].as<String>();

    doc.clear();
    file.close();

    return result;
}

bool settings_load_array(std::function<bool(JsonVariant&)> parse, const char* elementName, const char* filePath = "/default.json")
{
    File file = LittleFS.open(filePath, FILE_READ);

    if (!file)
    {
        log_e("There was an error opening %s file", filePath);
        return false;
    }
    
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {       
        doc.clear();
        file.close();
        return false;
    }

    auto root = doc.as<JsonObject>();

    auto array = root[elementName].as<JsonArray>();

    auto i = 0;

    for (JsonVariant element : array) 
    {
        if(!parse(element))
        {
            log_e("Parsing error on %s", element.as<String>());
            break;
        }
    }

    file.close();

    return true;
}

void settings_save(uint8_t *data, size_t len, size_t index, size_t total, const char* filePath = "/default.json")
{
    File file = LittleFS.open(filePath, "w+");
    file.write(data, total);
    file.close();
}