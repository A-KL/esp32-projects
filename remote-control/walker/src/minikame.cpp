#include "minikame.h"

int angToUsec(float value){
    return value/180 * (MAX_PULSE_WIDTH-MIN_PULSE_WIDTH) + MIN_PULSE_WIDTH;
}

void MiniKame::init(){


    board_pins[0] = D1; // front left inner
    board_pins[1] = D4, // front right inner
    board_pins[2] = D8; // front left outer
    board_pins[3] = D6; // front right outer
    board_pins[4] = D7; // back left inner
    board_pins[5] = D5; // back right inner
    board_pins[6] = D2; // back left outer
    board_pins[7] = D3; // back right outer
    board_pins[8] = D0; // camera
    
    // inner: 0, 1, 4, 5

    trim[0] = -5;
    trim[1] = 5;
    trim[2] = -3;
    trim[3] = 7;
    trim[4] = 7;
    trim[5] = -8;
    trim[6] = 5;
    trim[7] = -5;
    trim[8] = 1;
    
    for (int i=0; i<9; i++) reverse[i] = 0;


    for(int i=0; i<9; i++) oscillator[i].setTrim(trim[i]);
    for(int i=0; i<9; i++) servo[i].attach(board_pins[i]);
}

void MiniKame::turnR(float steps, float T=600){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 15;
    int hi = 23;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,0};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90+45};
    int phase[] = {0,180,90,90,180,0,90,90,0};

    execute(steps, period, amplitude, offset, phase);
        
}

void MiniKame::turnL(float steps, float T=600){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 15;
    int hi = 23;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,0};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90-45};
    int phase[] = {180,0,90,90,0,180,90,90,0};

    execute(steps, period, amplitude, offset, phase);
        
}

void MiniKame::dance(float steps, float T=600){
    int x_amp = 0;
    int z_amp = 40;
    int ap = 30;
    int hi = 20;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    int phase[] = {0,0,0,270,0,0,90,180,0};

    execute(steps, period, amplitude, offset, phase);
    
}

void MiniKame::frontBack(float steps, float T=600){
    int x_amp = 30;
    int z_amp = 25;
    int ap = 20;
    int hi = 30;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    int phase[] = {0,180,270,90,0,180,90,270,0};

    execute(steps, period, amplitude, offset, phase);
    
}

void MiniKame::run(float steps, float T=5000){
    int x_amp = 25;
    int z_amp = 15;
    int ap = 15;
    int hi = 15;
    int front_x = 9;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,10};
    int offset[] = {    90+ap-front_x,
                        90-ap+front_x,
                        90-hi,
                        90+hi,
                        90-ap-front_x,
                        90+ap+front_x,
                        90+hi,
                        90,
                        90
                    };
    int phase[] = {0,0,90,90,180,180,90,90,0};

    execute(steps, period, amplitude, offset, phase);
        
}

void MiniKame::runback(float steps, float T=5000){
    int x_amp = 25;
    int z_amp = 15;
    int ap = 15;
    int hi = 15;
    int front_x = 9;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,10};
    int offset[] = {    90+ap-front_x,
                        90-ap+front_x,
                        90-hi,
                        90+hi,
                        90-ap-front_x,
                        90+ap+front_x,
                        90+hi,
                        90
                    };
    int phase[] = {0,0,90,90,180,180,90,90};

    execute(steps, period, amplitude, offset, phase);
        
}

void MiniKame::moonwalkL(float steps, float T=5000){
    int z_amp = 45;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {0,0,z_amp,z_amp,0,0,z_amp,z_amp};
    int offset[] = {90, 90, 90, 90, 90, 90, 90, 90, 90-96};
    int phase[] = {0,0,0,120,0,0,180,290,0};

    execute(steps, period, amplitude, offset, phase);

}

void MiniKame::walk(float steps, float T=5000){
    volatile int x_amp = 20;
    volatile int z_amp = 15;
    volatile int ap = 20;
    volatile int hi = 10;
    volatile int front_x = 12;
    volatile float period[] = {T, T, T/2, T/2, T, T, T/2, T/2, T};
    volatile int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,5};
    volatile int offset[] = {   90+ap-front_x,
                                90-ap+front_x,
                                90-hi,
                                90+hi,
                                90-ap-front_x,
                                90+ap+front_x,
                                90+hi,
                                90-hi,
                                90
                    };
    volatile int  phase[] = {90, 90, 270, 90, 270, 270, 90, 270, 0};

    for (int i=0; i<9; i++){
        oscillator[i].reset();
        oscillator[i].setPeriod(period[i]);
        oscillator[i].setAmplitude(amplitude[i]);
        oscillator[i].setPhase(phase[i]);
        oscillator[i].setOffset(offset[i]);
    }

    _final_time = millis() + period[0]*steps;
    _init_time = millis();
    bool side;
    while (millis() < _final_time){
        side = (int)((millis()-_init_time) / (period[0]/2)) % 2;
        setServo(0, oscillator[0].refresh());
        setServo(1, oscillator[1].refresh());
        setServo(4, oscillator[4].refresh());
        setServo(5, oscillator[5].refresh());
        
        if (side == 0){
            setServo(3, oscillator[3].refresh());
            setServo(6, oscillator[6].refresh());
        }
        else{
            setServo(2, oscillator[2].refresh());
            setServo(7, oscillator[7].refresh());
        }
        delay(1);
    }
        
}

void MiniKame::walkback(float steps, float T=5000){
    volatile int x_amp = 20;
    volatile int z_amp = 15;
    volatile int ap = 20;
    volatile int hi = 10;
    volatile int back_x = 12;
    volatile float period[] = {T, T, T/2, T/2, T, T, T/2, T/2, T};
    volatile int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp,5};
    volatile int offset[] = {   90+ap+back_x,
                                90-ap-back_x,
                                90-hi,
                                90+hi,
                                90-ap+back_x,
                                90+ap-back_x,
                                90+hi,
                                90-hi,
                                90
                    };
    volatile int  phase[] = {270, 270, 270, 90, 90, 90, 90, 270, 0};

    for (int i=0; i<9; i++){
        oscillator[i].reset();
        oscillator[i].setPeriod(period[i]);
        oscillator[i].setAmplitude(amplitude[i]);
        oscillator[i].setPhase(phase[i]);
        oscillator[i].setOffset(offset[i]);
    }

    _final_time = millis() + period[0]*steps;
    _init_time = millis();
    bool side;
    while (millis() < _final_time){
        side = (int)((millis()-_init_time) / (period[0]/2)) % 2;
        setServo(0, oscillator[0].refresh());
        setServo(1, oscillator[1].refresh());
        setServo(4, oscillator[4].refresh());
        setServo(5, oscillator[5].refresh());
        
        if (side == 0){
            setServo(3, oscillator[3].refresh());
            setServo(6, oscillator[6].refresh());
        }
        else{
            setServo(2, oscillator[2].refresh());
            setServo(7, oscillator[7].refresh());
        }
        delay(1);
    }
        
}

void MiniKame::upDown(float steps, float T=5000){
    int x_amp = 0;
    int z_amp = 35;
    int ap = 20;
    int hi = 25;
    int front_x = 0;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {    90+ap-front_x,
                        90-ap+front_x,
                        90-hi,
                        90+hi,
                        90-ap-front_x,
                        90+ap+front_x,
                        90+hi,
                        90-hi,
                        90
                    };
    int phase[] = {0,0,90,270,180,180,270,90,0};

    execute(steps, period, amplitude, offset, phase);

}


void MiniKame::pushUp(float steps, float T=600){
    int z_amp = 40;
    int x_amp = 65;
    int hi = 30;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {0,0,z_amp,z_amp,0,0,0,0};
    int offset[] = {90,90,90-hi,90+hi,90-x_amp,90+x_amp,90+hi,90-hi,90};
    int phase[] = {0,0,0,180,0,0,0,0,0};

    execute(steps, period, amplitude, offset, phase);

}

void MiniKame::hello(){
      
    int T=800; // was 350
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {0,40,0,40,0,0,0,0,2};
    int offset[] = {90+15,40,90-65,90,90+10,90-10,90+20,90-20,90};
    int phase[] = {0,0,0,90,0,0,0,0,0};

    execute(4, period, amplitude, offset, phase);
    
}

void MiniKame::bye(){
   
    int T=800; // was 350
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {40,40,90,90,0,0,0,2};
    int offset[] = {90+50,90-50,90-15,90+15,90+20,90-20,90+10,90-10,90};
    int phase[] = {0,0,90,90,0,0,0,0,0};

    execute(4, period, amplitude, offset, phase);
    
}

void MiniKame::afraid(){
    float afraidPositionLeft[]={90+15,90-15,90-65,90+65,90+20,90-20,90+10,90-10,90-3};
    moveServos(150, afraidPositionLeft);
    float afraidPositionRight[]={90+15,90-15,90-65,90+65,90+20,90-20,90+10,90-10,90+3};
    moveServos(150, afraidPositionRight);
}

void MiniKame::confused(){ //Up
    int ap = 20;
    int hi = 35;  
    int up = 30 + random(5,20);  
    int down = 5 + random(5,20);
    float confusedPositionA[]={90+ap,90-ap,90-hi+up,90+hi+down,90-ap,90+ap,90+hi-up,90-hi-down,90+5};
    moveServos(random(200,700), confusedPositionA);
    delay(random(800,1400));
    float confusedPositionB[]={90+ap,90-ap,90-hi-down,90+hi-up,90-ap,90+ap,90+hi+down,90-hi+up,90-5};
    moveServos(random(200,700), confusedPositionB);    
    delay(random(800,1400));
}

void MiniKame::shakeYes(float steps, float T=400){ //Yes
    int z_amp = 20;
    int x_amp = 30;
    int ap = 25;
    int hi = 30;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {0,0,z_amp,z_amp,0,0,0,0};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    int phase[] = {0,0,0,180,0,0,0,0,0};

    execute(steps, period, amplitude, offset, phase);
    
}

void MiniKame::headbang(float steps, float T=400){ //headbang
    int z_amp = 30;
    int x_amp = 30;
    int ap = 25;
    int hi = 30;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {0,0,z_amp,z_amp,0,0,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    int phase[] = {0,0,0,180,0,0,0,180,0};

    execute(steps, period, amplitude, offset, phase);
    
}

void MiniKame::shakeNo(){ //No
    int ap = 20;
    int hi = 35;
    float shakeNoPositionLeft[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90-45};
    moveServos(100, shakeNoPositionLeft);

    float shakeNoPositionRight[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90+45};
    moveServos(100, shakeNoPositionRight);

}

void MiniKame::lookLeft(){ //Left
    int ap = 20;
    int hi = 35;
    float lookLeftPosition[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90-45};
    moveServos(150, lookLeftPosition);
}

void MiniKame::lookRight(){ //Right
    int ap = 20;
    int hi = 35;
    float lookRightPosition[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90+45};
    moveServos(150, lookRightPosition);
}

void MiniKame::lookUp(){ //Up
    int ap = 20;
    int hi = 35;    
    float lookUpPosition[]={90+ap,90-ap,90-hi-35,90+hi+35,90-ap,90+ap,90+hi-35,90-hi+35,90};
    moveServos(150, lookUpPosition);
}

void MiniKame::lookUpLeft(){ //Up
    int ap = 20;
    int hi = 35;    
    float lookUpLeftPosition[]={90+ap,90-ap,90-hi-35,90+hi+35,90-ap,90+ap,90+hi-35,90-hi+35,90-45};
    moveServos(150, lookUpLeftPosition);
}

void MiniKame::lookUpRight(){ //Up
    int ap = 20;
    int hi = 35;    
    float lookUpRightPosition[]={90+ap,90-ap,90-hi-35,90+hi+35,90-ap,90+ap,90+hi-35,90-hi+35,90+45};
    moveServos(150, lookUpRightPosition);
}

void MiniKame::lookDown(){ //Down
    int ap = 20;
    int hi = 35;    
    float lookDownPosition[]={90+ap,90-ap,90-hi+35,90+hi-35,90-ap,90+ap,90+hi+35,90-hi-35,90};
    moveServos(150, lookDownPosition);
}

void MiniKame::lookDownLeft(){ //Down
    int ap = 20;
    int hi = 35;    
    float lookDownLeftPosition[]={90+ap,90-ap,90-hi+35,90+hi-35,90-ap,90+ap,90+hi+35,90-hi-35,90-45};
    moveServos(150, lookDownLeftPosition);
}

void MiniKame::lookDownRight(){ //Down
    int ap = 20;
    int hi = 35;    
    float lookDownRightPosition[]={90+ap,90-ap,90-hi+35,90+hi-35,90-ap,90+ap,90+hi+35,90-hi-35,90+45};
    moveServos(150, lookDownRightPosition);
}

void MiniKame::center(){
    int ap = 20;
    int hi = 35;    
    float centerPosition[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    moveServos(150, centerPosition);

}

void MiniKame::rest(){
    int ap = 20;
    int hi = -15;    
    float restPosition[]={90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    moveServos(150, restPosition);
}

void MiniKame::home(){
    int ap = 20;//20
    int hi = 35;//35    
    int position[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    for (int i=0; i<9; i++) setServo(i, position[i]);
}

void MiniKame::pie(){
    int ap = 20;
    int hi = 35;    
    float piePositionA[]={90+ap,90-ap+15,90-hi+60,90+hi+30,90-ap-45,90+ap-30,90+hi+10,90-hi+90,90-5};
    moveServos(150, piePositionA);
    float piePositionB[]={90+ap,90-ap+15,90-hi+60,90+hi+30,90-ap-45,90+ap-15,90+hi+10,90-hi+90,90-5};
    moveServos(150, piePositionB);   
}

void MiniKame::calibrate(){
    int ap = 0;
    int hi = -15;    
    int position[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi,90};
    for (int i=0; i<9; i++) setServo(i, position[i]);
}

void MiniKame::zero(){
    for (int i=0; i<9; i++) setServo(i, 90);
}

void MiniKame::reverseServo(int id){
    if (reverse[id])
        reverse[id] = 0;
    else
        reverse[id] = 1;
}


void MiniKame::setServo(int id, float target){
    if (!reverse[id])
        servo[id].writeMicroseconds(angToUsec(target+trim[id]));
    else
        servo[id].writeMicroseconds(angToUsec(180-(target+trim[id])));
    _servo_position[id] = target;
}

float MiniKame::getServo(int id){
    return _servo_position[id];
}


void MiniKame::moveServos(int time, float target[9]) {
    if (time>10){
        for (int i = 0; i < 9; i++)	_increment[i] = (target[i] - _servo_position[i]) / (time / 10.0);
        _final_time =  millis() + time;

        while (millis() < _final_time){
            _partial_time = millis() + 10;
            for (int i = 0; i < 9; i++) setServo(i, _servo_position[i] + _increment[i]);
            while (millis() < _partial_time); //pause
        }
    }
    else{
        for (int i = 0; i < 9; i++) setServo(i, target[i]);
    }
    for (int i = 0; i < 9; i++) _servo_position[i] = target[i];
}

void MiniKame::execute(float steps, float period[9], int amplitude[9], int offset[9], int phase[9]){

    for (int i=0; i<9; i++){
        oscillator[i].setPeriod(period[i]);
        oscillator[i].setAmplitude(amplitude[i]);
        oscillator[i].setPhase(phase[i]);
        oscillator[i].setOffset(offset[i]);
    }

    unsigned long global_time = millis();

    for (int i=0; i<9; i++) oscillator[i].setTime(global_time);

    _final_time = millis() + period[0]*steps;
    while (millis() < _final_time){
        for (int i=0; i<9; i++){
            setServo(i, oscillator[i].refresh());
        }
        yield();
    }
}
