#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_version.h>

/*
SDL_INIT_AUDIO: audio subsystem; automatically initializes the events subsystem
SDL_INIT_VIDEO: video subsystem; automatically initializes the events subsystem, should be initialized on the main thread.
SDL_INIT_JOYSTICK: joystick subsystem; automatically initializes the events subsystem
SDL_INIT_HAPTIC: haptic (force feedback) subsystem
SDL_INIT_GAMEPAD: gamepad subsystem; automatically initializes the joystick subsystem
SDL_INIT_EVENTS: events subsystem
SDL_INIT_SENSOR: sensor subsystem; automatically initializes the events subsystem
SDL_INIT_CAMERA: camera subsystem; automatically initializes the events subsystem
*/
#define SDL_INIT_EVERYTHING (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_SENSOR)

int main(int argc, char *argv[]) 
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int finished = 0;
    SDL_Event event;
    SDL_FRect rect;
    int ret;
    const int compiled = SDL_VERSION;  /* hardcoded number from SDL headers */
    const int linked = SDL_GetVersion();  /* reported by linked SDL library */

    SDL_Log("Compiled against SDL version %d.%d.%d ...\n",
        SDL_VERSIONNUM_MAJOR(compiled),
        SDL_VERSIONNUM_MINOR(compiled),
        SDL_VERSIONNUM_MICRO(compiled));

    SDL_Log("Linked against SDL version %d.%d.%d.\n",
        SDL_VERSIONNUM_MAJOR(linked),
        SDL_VERSIONNUM_MINOR(linked),
        SDL_VERSIONNUM_MICRO(linked));	

    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0");
    // SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL_Init failed (%s)", SDL_GetError());
        return 1;
    }

    SDL_Log("SDL information:");
    const char *Hints[] = {
    SDL_HINT_FRAMEBUFFER_ACCELERATION, 
    SDL_HINT_RENDER_DRIVER, 
    SDL_HINT_RENDER_VSYNC, 
    SDL_HINT_VIDEO_DOUBLE_BUFFER,
    };

    for (int i = 0; i < sizeof(Hints) / sizeof(char *); i++) {
      SDL_Log("    %s: %s\n", Hints[i], SDL_GetHint(Hints[i]));
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    if (SDL_CreateWindowAndRenderer("SDL3 info", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowTitle(window, "object detection opengles2");
    SDL_SetWindowPosition(window, 960, 540);
    //SDL_ShowWindow(window);

    while (1) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:  {
                   finished = 1;
                   SDL_Log("Program quit after %ld ticks", event.quit.timestamp);
                   break;
                }
                case SDL_EVENT_KEY_DOWN:  {
                  int withControl = !!(event.key.key & SDL_KMOD_CTRL);
                  int withShift = !!(event.key.key & SDL_KMOD_SHIFT);
                  int withAlt = !!(event.key.key & SDL_KMOD_ALT);

                  switch (event.key.key) {
                     /* Add hotkeys here */
                     case SDLK_ESCAPE:
                        finished = 1;
                        break;
                     case SDLK_UP:
                        finished = 1;
                        break;
                  }
                }
            }
        }
        if (finished) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 80, 80, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        rect.x = 50;
        rect.y = 50;
        rect.w = 300;
        rect.h = 300;
        ret = SDL_RenderRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Log("Program end");
    SDL_Quit();
}
