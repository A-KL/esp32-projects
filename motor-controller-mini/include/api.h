#ifndef api_h
#define api_h

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer api(8080);

void OnNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void api_init() 
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "RC Motor Controller");
    });

    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/default.json", "application/json");
    });

    //server.on("/config", HTTP_PUT, handle_led1on);

    server.onNotFound(OnNotFound);

    api.begin();
}

void api_loop()
{

}



#endif