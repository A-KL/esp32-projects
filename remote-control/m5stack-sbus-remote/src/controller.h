#pragma once

#ifdef INPUT_PS3
  #include <Ps3Controller.h>
#elif INPUT_PS4
  #include <PS4Controller.h>
#elif INPUT_XBOX
  #include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#endif

const static int dead_zone = 20;

bool controller_init()
{
#ifdef INPUT_PS3
    Ps3.begin("b8:27:eb:df:b3:ff");
    return Ps3.isConnected();
#elif INPUT_PS4
  PS4.begin("b8:27:eb:df:b3:ff");
  return PS4.isConnected();
#elif INPUT_XBOX
    xboxController.begin();
    return xboxController.isConnected();
#else
    log_i("No controller");
    return false;
#endif
}

bool controller_loop(int16_t* inputs)
{
#ifdef INPUT_PS3
    if (!Ps3.isConnected()) 
    {
      ps_values.clearValues();    
      return false;
    }
    
    Ps3.setPlayer(1);

    inputs[0] = Ps3.data.analog.button.r2;
    inputs[1] = Ps3.data.analog.button.l2;
    inputs[2] = Ps3.data.analog.stick.lx;
    inputs[3] = Ps3.data.analog.stick.ly;
    inputs[4] = Ps3.data.analog.button.l1;
    inputs[5] = Ps3.data.analog.button.l2;
    inputs[6] = Ps3.data.analog.stick.rx;
    inputs[7] = Ps3.data.analog.stick.ry;

    for (uint8_t i = 0; i < 8; i++) {
      ps_values.setValue(i, inputs[i]);
    }

    // auto power = Ps3.data.analog.button.r2 - Ps3.data.analog.button.l2;
    // auto steer = - Ps3.data.analog.stick.lx;

    // if (abs(Ps3.data.analog.stick.ly) > dead_zone) {
    //   left_speed = map(Ps3.data.analog.stick.ly, -128, 128, -255, 255);
    // }

    // if (abs(Ps3.data.analog.stick.ry) > dead_zone) {
    //   right_speed = map(Ps3.data.analog.stick.ry, -128, 128, 255, -255);
    // }

    return true;
#else
    return false;
#endif
}