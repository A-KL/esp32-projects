#include "commandexecutor.h"

void CommandExecutor::init(MiniKame* kame) {
  robot = kame;
  running = false;
}

void CommandExecutor::parseCommand(String command) {
  switch (command.toInt()){

    case 1: // UP
    robot->walk(1,450); //450
    running = 1;
    break;
            
    case 2: //CENTER
    robot->center();
    robot->home();
    break;

    case 3: //LEFT
    robot->turnL(1,450); //450
    running = 1;
    break;

    case 4: //RIGHT
    robot->turnR(1,450); //450
    running = 1;
    break;

    case 5: //home
    running = 0;
    robot->home();
    //delay(50);
    break;

    case 6: //lookUp
    robot->lookUp();
    break;
    
    case 7: //lookDown
    robot->lookDown();
    break;

    case 8: //lookLeft
    robot->lookLeft();
    break;
    
    case 9: //lookRight
    robot->lookRight();
    break;    

    case 10: //shakeYes
    robot->shakeYes(2,200);
    break; 

    case 11: //shakeNo
    robot->shakeNo();
    break; 
    
    case 12: //PUSHUP
    robot->pushUp(2,1600);
    break;

    case 13: //HAPPY
    robot->upDown(4,200);// 4,400
    break;

    case 14: // DOWN
    robot->walkback(1,450); //450
    running = 1;
    break;

    case 15: //HELLO
    robot->hello();
    break;

    case 16: //SKATE
    robot->frontBack(4,300);//200
    break;

    case 17: //DANCE
    robot->dance(2,800);
    break;

    case 18: //MOONWALK
    robot->moonwalkL(2,1600);
    break;

    case 19: //REST
    robot->rest();
    break;

    case 20: //PIE
    robot->pie();
    break;    

    case 21: //LookUpLeft
    robot->lookUpLeft();
    break;

    case 22: //LookUpRight
    robot->lookUpRight();
    break;

    case 23: //LookDownLeft
    robot->lookDownLeft();
    break;

    case 24: //LookDownRight
    robot->lookDownRight();
    break;

    case 25: //calibrate
    robot->calibrate();
    break;
    
    case 26: //afraid
    robot->afraid();
    break;

    case 27: //bye
    robot->bye();
    break;

    case 28: //confused
    robot->confused();
    break;    

    case 29: // UP fast
    robot->run(1,300); //450
    running = 1;
    break;

    case 30: //Headbang
    robot->headbang(2,250);    
    break;
        
    default:
    robot->home();
    //delay(50);
    break;

  }
}
