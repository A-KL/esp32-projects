#pragma once

#ifdef ARDUINO
  #include <esp_pthread.h>
#else
  #include <pthread.h>
#endif

static pthread_t ui_pthread;
static bool ui_pthread_run = true;

void * update_ui_thread(void * p)
{
    while(ui_pthread_run) {
      form.Update(canvas);
    }
    return NULL;
}

void setupUI()
{
#ifdef ARDUINO
    esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    cfg.stack_size = (4 * 1024);
    cfg.pin_to_core = 0;
    esp_pthread_set_cfg(&cfg);
#endif
    ui_pthread_run = true;

    pthread_create(&ui_pthread, NULL, update_ui_thread, nullptr);

  // xTaskCreatePinnedToCore(
  //                   loopUI,         /* Task function. */
  //                   "UI",           /* name of task. */
  //                   20000,          /* Stack size of task */
  //                   args,           /* parameter of the task */
  //                   1,              /* priority of the task */
  //                   &uiHandle,      /* Task handle to keep track of created task */
  //                   0);             /* pin task to core 0 */                  
 // delay(500);
}

inline void loopUI()
{ }

void cleanup() {
  ui_pthread_run = false;
  pthread_join(ui_pthread, nullptr);
}