#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

typedef struct __attribute__((packed)) {
    char name[20];
} SpotifyArtist;

typedef struct __attribute__((packed)) {
    SpotifyArtist artist;
    char name[20];
    unsigned long duration_ms;
    
} SpotifyItem;

typedef struct __attribute__((packed)) {
    SpotifyItem item;
    unsigned char is_playing;
    unsigned long progress_ms;
} SpotifyTrack;

class SpotifyClient
{
public:
    SpotifyClient(){};

    bool GetCurrentTrack(SpotifyTrack& track) const
    {
        auto json_content = GetAndParse(_currentPlaying);

        SpotifyTrack result;

        result.progress = json_content[""];
    }
private:
    StaticJsonBuffer<3000> _json;

    bool GetAndParse(const char* path, const JSONVar& result)
    {
        http.begin(path);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Accept", "application/json");
        http.addHeader("Authorization", "Bearer");

        int responseCode = http.GET();

        Serial.print("HTTP Response code: ");
        Serial.println(responseCode);

        if (responseCode != 200)
        {
            http.end();
            return false;
        }
     
        auto payload = http.getString();
        auto parsed = _json.parseObject(payload);

        http.end();
        
        if (!parsed.success())
        {
            return false;
        }

        &result = parsed;

        return true;
    }

    const char* _currentPlaying = "https://api.spotify.com/v1/me/player/currently-playing";
};