// Copyright 2021 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <stdio.h>

#include "window.h"
#include "input.h"
#include "sounds.h"
#include "graphics.h"
#include "globals.h"

void Render();
void ClearScreen(RGB color);
void PrintSDLError();
void OnQuit();
bool CreateWindow(char* title, uint32_t width, uint32_t height);
bool CreateRenderer();

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool InitializeWindow(char* title, uint32_t width, uint32_t height) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        PrintSDLError();
        return false;
    }
    if (!CreateWindow(title, width, height) || !CreateRenderer()) {
        return false;
    }
    InitializeMixer();
    is_running = true;
    return true;
}

SDL_Renderer* GetRenderer() {
    return renderer;
}

void DeinitWindow() {
    if (window != NULL) SDL_DestroyWindow(window);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void StartGameLoop(void (*game_loop)()) {
    uint32_t tick_start = GetTicks();
    uint32_t tick_now = 0;

    while(is_running) {
        tick_now = GetTicks();
        uint32_t delta = tick_now - tick_start;

        if (delta > FPS / 1000) {
            PollEvents(OnQuit);
            ClearScreen(HexToRGB(0xf7f7f7));
            game_loop();
            Render();
        }
    }
}

void ClearScreen(RGB color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(renderer);
}

void Render() {
    SDL_RenderPresent(renderer);
}

uint32_t GetTicks() {
    return SDL_GetTicks();
}

bool CreateWindow(char* title, uint32_t width, uint32_t height) {
    window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        PrintSDLError();
        return false;
    }
    return true;
}

bool CreateRenderer() {
    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_SOFTWARE
    );
    if (renderer == NULL) {
        PrintSDLError();
        return false;
    }
    return true;
}

void OnQuit() {
    is_running = false;
}

void PrintSDLError() {
    printf("SDL Error: %s\n", SDL_GetError());
}