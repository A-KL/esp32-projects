#include <Arduino.h>
#include <Ramp.h> 

// CONSTS

#define RESOLUTION_PWM_BITS   8
#define RESOLUTION_ADC_BITS   10

#define INPUT_ADC_MIN         0
#define INPUT_ADC_MAX         1023

#define OUTPUT_PWM_MIN        0
#define OUTPUT_PWM_MAX        255

// CONFIG

#ifndef INPUT_ADC_DEADZONE
#define INPUT_ADC_DEADZONE    10
#endif

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

#ifndef MOTOR_PWM_FQC
#define MOTOR_PWM_FQC         20000 // Hz
#endif

// PINS

#ifndef INPUT_ADC_PIN
#define INPUT_ADC_PIN         5
#endif

#ifndef OUTPUT_DIR_PIN
#define OUTPUT_DIR_PIN        1
#endif

#ifndef OUTPUT_PWM_PIN
#define OUTPUT_PWM_PIN        2
#endif

rampInt motor_speed;

void setup() 
{
  Serial.begin(115200);

  pinMode(OUTPUT_DIR_PIN, OUTPUT);

  analogSetWidth(RESOLUTION_ADC_BITS);
  analogReadResolution(RESOLUTION_ADC_BITS);

  ledcSetup(0, MOTOR_PWM_FQC, RESOLUTION_PWM_BITS);
  ledcAttachPin(OUTPUT_PWM_PIN, 0);
}

void loop() 
{
    auto input_raw = analogRead(INPUT_ADC_PIN);
    auto thr = (INPUT_ADC_MAX - INPUT_ADC_MIN) / 2.0  - input_raw;
    
    auto output = 0;

    if ( abs(thr) >  INPUT_ADC_DEADZONE)
    {
      log_d("Speed:\t%d\t%d", output, output);
      output = motor_speed.update();
      //output = map(abs(input_raw), INPUT_ADC_MIN, INPUT_ADC_MAX, OUTPUT_PWM_MIN, OUTPUT_PWM_MAX);
    }

    Serial.print(input_raw);
    Serial.print('\t');
    Serial.println(output);

    ledcWrite(0, output);

    delay(50);
}