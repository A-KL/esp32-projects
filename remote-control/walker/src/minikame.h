#pragma once

#include <ESP32Servo.h>

#include "octosnake.h"

class MiniKame {

    public:
        void init();
        void run(float steps, float period);
        void walk(float steps, float period);
        void runback(float steps, float period);
        void turnL(float steps, float period);
        void turnR(float steps, float period);
        void moonwalkL(float steps, float period);
        void dance(float steps, float period);
        void upDown(float steps, float period);
        void pushUp(float steps, float period);
        void hello();
        void walkback(float steps, float period);
        void lookDown();
        void lookUp();
        void lookUpRight();
        void lookUpLeft();
        void lookDownRight();
        void lookDownLeft();        
        void lookLeft();
        void lookRight();
        void shakeNo();
        void calibrate();
        void afraid();
        void confused();
        void bye();        
        void shakeYes(float steps, float period);
        void headbang(float steps, float period);
        void home();
        void center();
        void rest();
        void pie();
        void zero();
        void frontBack(float steps, float period);

        void setServo(int id, float target);
        void reverseServo(int id);
        float getServo(int id);
        void moveServos(int time, float target[9]);

    private:
        Oscillator oscillator[9];
        Servo servo[9];
        int board_pins[9];
        int trim[9];
        bool reverse[9];
        unsigned long _init_time;
        unsigned long _final_time;
        unsigned long _partial_time;
        float _increment[9];
        float _servo_position[9];

        //int angToUsec(float value);
        void execute(float steps, float period[9], int amplitude[9], int offset[9], int phase[9]);
};