#include <unistd.h>
#include <iostream>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"

static void delay(unsigned long miliseconds)
{
    SDL_Delay(miliseconds);
}

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void * data)
{
    (void)data;

    while(1) {
        delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

float hal_get_altitude()
{
  return rand()%1000 + 2000;
}

float hal_get_pitch()
{
  return rand()%10 + 5;;
;
}

void hal_log_cb(const char* buf)
{
  std::cout << "LVGL Log: " << buf << std::endl;
}

void hal_setup(void)
{
    // Workaround for sdl2 `-m32` crash
    // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
    #ifndef WIN32
        setenv("DBUS_FATAL_WARNINGS", "0", 1);
    #endif

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);

    //lv_tick_set_cb(SDL_GetTicks)
}

void hal_loop(void)
{
    delay(5);
    lv_task_handler();
}