#pragma once

#ifdef ARDUINO
  #include <esp_pthread.h>
#else
  #include <pthread.h>
#endif
  #include <pthread.h>

static pthread_t system_pthread;
static bool system_pthread_run = true;

// #include "AudioTools/Concurrency/RTOS.h"
// Task task("write", 5 * 1024, 10, 0);

void* run_system_thread(void * args)
{
  //auto canvas = (Canvas<Color>*)args;
    while (system_pthread_run) 
    {
      loopAudio(); 
      delay(15);
      // if (form.IsValid()){
      //   delay(5);
      //   continue;
      // }
      // form.Update(canvas);
    }

   return NULL;
}

void startTasks(void * args = NULL)
{
#ifdef ARDUINO
    esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    cfg.stack_size = (10 * 1024);
    cfg.pin_to_core = 0;
    cfg.prio = 10;
    esp_pthread_set_cfg(&cfg);
#endif
    system_pthread_run = true;
    pthread_create(&system_pthread, NULL, run_system_thread, nullptr);

  // xTaskCreatePinnedToCore(
  //                   update_ui_thread, /* Task function. */
  //                   "UI",             /* name of task. */
  //                   15 * 1024,        /* Stack size of task */
  //                   args,             /* parameter of the task */
  //                   0,                /* priority of the task */
  //                   &ui_handle,       /* Task handle to keep track of created task */
  //                   0);               /* pin task to core 0 */      
  
   //task.begin([](){loopAudio(); delay(5);});
}

void cleanup() {
  system_pthread_run = false;
  pthread_join(system_pthread, nullptr);
}