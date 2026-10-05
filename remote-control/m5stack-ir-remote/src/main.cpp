#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <M5StickCPlus2.h>
/*
  http://www.remotecentral.com/cgi-bin/mboard/rc-discrete/thread.cgi?5780
  https://github.com/z3t0/Arduino-IRremote/issues/263
  
  IR Codes
  
  Samsung TV
  Power SAMSUNG 0xE0E09966  On Only
  Power SAMSUNG 0xE0E040BF  Toggle
  CH+   SAMSUNG 0xE0E048B7
  CH-   SAMSUNG 0xE0E008F7
  V+    SAMSUNG 0xE0E0E01F
  V-    SAMSUNG 0xE0E0D02F
  Mute  SAMSUNG 0xE0E0F00F
  0xE0E0F00F = Mute
  0xE0E0E01F = Vol+
  0xE0E0D02F = Vol-
  0xE0E0BE41 = TV
  0xE0E0A35C = HDMI4 (Android)
  0xE0E09966 = Power On
  0xE0E09768 = HDMI1
  0xE0E09669 = PC
  0xE0E07D82 = HDMI2 (Satellite)
  0xE0E0619E = Component
  0xE0E043BC = HDMI3 (Freeview)
  0xE0E040BF = Power Toggle
  0xE0E037C8 = AV
  0xE0E021DE = EXT1
  0xE0E019E6 = Power Off
*/

#define BUTTERY_FULL 4200
#define BUTTERY_LOW 3100

// #define SAMSUNG_POWER_ON 0xE0E09966 ???
#define SAMSUNG_POWER_TOGGLE 0xE0E06798

static IRsend ir_send(IR_OUT_PIN);
static IRrecv ir_recv(IR_IN_PIN);
static decode_results ir_results;

void setup() {
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  StickCP2.Display.setRotation(1);
  StickCP2.Display.setTextColor(GREEN);
  StickCP2.Display.setTextDatum(middle_center);
  StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
  StickCP2.Display.setTextSize(1);

  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  ir_send.begin();
  ir_recv.enableIRIn();
}

static int last_battery_voltage = 0;

void loop() 
{
  int vol = StickCP2.Power.getBatteryVoltage();

  if (last_battery_voltage != vol) 
  {
    last_battery_voltage = vol;

    StickCP2.Display.clear();
    StickCP2.Display.setCursor(10, 30);
    StickCP2.Display.printf("BAT: %dmv\r\n%d%%", vol, map(vol, BUTTERY_LOW, BUTTERY_FULL, 0, 100));
  }

  StickCP2.update();

  if (StickCP2.BtnA.wasPressed()) {
    ir_send.sendSAMSUNG(SAMSUNG_POWER_TOGGLE, 32);
    ir_send.sendSAMSUNG(0xE0E040BF, 32);
    Serial.println("POWER TOGGLE");
  } 
  
  if (StickCP2.BtnB.wasPressed()) {
    ir_send.sendSAMSUNG(0xE0E0E01F, 32);
    Serial.println("Volume UP");
  }

  if (ir_recv.decode(&ir_results)) {
    // digitalWrite(LED_PIN, HIGH);
    Serial.printf("IR: 0x%llX\n", (unsigned long long)ir_results.value);
    ir_recv.resume();
    // digitalWrite(LED_PIN, LOW);
  }
}