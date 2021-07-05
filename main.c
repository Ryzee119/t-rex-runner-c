// Copyright 2021 Betamark Pty Ltd. All rights reserved.
// Author: Shloi Nissan (shlomi@betamark.com)

#include <stdlib.h>

#include "game.h"
#include "globals.h"
#include "sys/window.h"
#include "sys/graphics.h"
#include "sys/sounds.h"
#ifdef NXDK
#include <hal/video.h>
#endif

bool Initialize() {
    return InitializeWindow(
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
}

bool LoadAssets() {
    if (LoadTexture("D:\\assets\\textures\\spritesheet.png") == -1) {
        return false;
    }
    LoadSoundEffect("D:\\assets\\sounds\\sfx-hit.wav");
    LoadSoundEffect("D:\\assets\\sounds\\sfx-press.wav");
    LoadSoundEffect("D:\\assets\\sounds\\sfx-reached.wav");
    return true;
}

void StartGame() {
    srand(GetTicks());
    InitGame();
    StartGameLoop(RunGame);
}

void FreeResources() {
    FreeTextures();
    FreeSoundEffects();
    DestroyGame();
    DeinitMixer();
    DeinitWindow();
}

int main(int argc, char *argv[]) {
#ifdef NXDK
    XVideoSetMode(720, 480, 32, REFRESH_DEFAULT);
#endif
    if (!Initialize() || !LoadAssets()) return -1;

    StartGame();

    FreeResources();

    return 0;
}
