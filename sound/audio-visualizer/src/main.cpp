#if defined ( LGFX_BACKEND )
#define PROGMEM 
#include <lgfx/v1/platforms/sdl/Panel_sdl.hpp>
#endif

#if defined ( SDL_h_ )

void setup(void);
void loop(void);
void cleanup(void);

__attribute__((weak))
int user_func(bool* running)
{
  setup();
  
  do {
    loop();
  } 
  while (*running);

  //cleanup();

  return 0;
}

int main(int, char**)
{
  return lgfx::Panel_sdl::main(user_func);
}

#endif