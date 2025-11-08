#pragma once

#ifdef ARDUINO
  #include <esp_pthread.h>
#else
  #include <pthread.h>
#endif

static TaskHandle_t ui_handle;
//static pthread_t ui_pthread;
static bool ui_pthread_run = true;

void update_ui_thread(void * args)
{
  //auto canvas = (Canvas<Color>*)args;
    while (ui_pthread_run) 
    {
      if (form.IsValid()){
        delay(5);
        continue;
      }
      form.Update(canvas);     
      delay(40);

    }
  //  return NULL;
}

void setupUI(void * args = NULL)
{
#ifdef ARDUINO
    // esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    // cfg.stack_size = (15 * 1024);
    // cfg.pin_to_core = 0;
    // esp_pthread_set_cfg(&cfg);
#endif
    ui_pthread_run = true;

   //pthread_create(&ui_pthread, NULL, update_ui_thread, nullptr);

  xTaskCreatePinnedToCore(
                    update_ui_thread, /* Task function. */
                    "UI",             /* name of task. */
                    15 * 1024,        /* Stack size of task */
                    args,             /* parameter of the task */
                    0,                /* priority of the task */
                    &ui_handle,       /* Task handle to keep track of created task */
                    0);               /* pin task to core 0 */                  
}

inline void loopUI()
{ }

void cleanup() {
  ui_pthread_run = false;
 // pthread_join(ui_pthread, nullptr);
}