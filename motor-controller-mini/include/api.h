#ifndef api_h
#define api_h

#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <storage.h>

AsyncWebServer api(8080);

void OnNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void api_init()  {

    api.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/default.json", "application/json");
    });

    api.on("/config", HTTP_PUT, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        try
        {
            if (!settings_apply(data, len, index, total))
            {
                request->send(400, "text/plain", "Fehlerhafte JSON Struktur");
                return;
            }
            request->send(200); 
        }
        catch(const std::exception& e)
        {
            request->send(500, "Failed to parse json");
        }
        try
        {
            settings_save(data, len, index, total);
            request->send(200);
        }
        catch(const std::exception& e)
        {
            request->send(500, "Failed to save json");
            return;
        }
    });

    api.onNotFound(OnNotFound);

    api.begin();
}

void api_loop()
{ }

#endif