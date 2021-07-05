// Copyright 2021 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <SDL2/SDL.h>

#include "input.h"

bool keyboard[256] = {false};
static SDL_GameController *pad = NULL;

void (*key_up_callback)(char key) = NULL;

void PollEvents(void (*on_quit)()) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) on_quit();

        if (event.type == SDL_CONTROLLERDEVICEADDED)
        {
            SDL_GameController *new_pad = SDL_GameControllerOpen(event.cdevice.which);
            if (pad == NULL)
            {
                pad = new_pad;
            }
        }
        else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
        {
            if (pad == SDL_GameControllerFromInstanceID(event.cdevice.which))
            {
                pad = NULL;
            }
            SDL_GameControllerClose(SDL_GameControllerFromInstanceID(event.cdevice.which));
        }
        else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
            {
                keyboard[KEY_UP] = true;
            }
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
            {
                keyboard[KEY_DOWN] = true;
            }
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                keyboard[KEY_SPACE] = true;
            }
        }
        else if (event.type == SDL_CONTROLLERBUTTONUP) {
            uint8_t key;
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
            {
                key = KEY_UP;
            }
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
            {
                key = KEY_DOWN;
            }
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                key = KEY_SPACE;
            }
            else
            {
                return;
            }
            keyboard[key] = false;
            if (key_up_callback != NULL) {
                key_up_callback(key);
            }
        }
    }
}

void SetOnKeyUp(void (*on_key_up)(char key)) {
    key_up_callback = on_key_up;
}

bool IsKeyDown(char key) {
    return keyboard[key];
}