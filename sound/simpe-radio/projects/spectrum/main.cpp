#if defined ( LGFX_BACKEND )
#define PROGMEM 
#include <lgfx/v1/platforms/sdl/Panel_sdl.hpp>
#endif

#if defined ( SDL_h_ )

void setup(void);
void loop(void);
// void cleanup(void);

__attribute__((weak))
int user_func(bool* running)
{
  setup();
  
  do {
    loop();
  } 
  while (*running);

  // cleanup();

  return 0;
}

int main(int, char**)
{
  // lgfx::Panel_sdl::addKeyCodeMapping(SDL_KeyCode::SDLK_m, VOLUME_BUTTON);
  // lgfx::Panel_sdl::addKeyCodeMapping(SDL_KeyCode::SDLK_UP, VOLUME_PIN_A);
  // lgfx::Panel_sdl::addKeyCodeMapping(SDL_KeyCode::SDLK_DOWN, VOLUME_PIN_B);

  return lgfx::Panel_sdl::main(user_func);
}

#endif