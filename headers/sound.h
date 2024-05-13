#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"
#include <bits/stdc++.h>
using namespace std;

enum SoundType {
  Hit, Attack, Death, Hover, Confirm, Pause, Unpause
};

class SoundMap {
public:
  static map<SoundType, Sound> soundMap;
  SoundMap() {}
  static void Init() {
    soundMap[Hit] = LoadSound("../assets/sound/RPG_Essentials_Free/12_Player_Movement_SFX/61_Hit_03.wav");
    soundMap[Attack] = LoadSound("../assets/sound/RPG_Essentials_Free/10_Battle_SFX/03_Claw_03.wav");
    soundMap[Death] = LoadSound("../assets/sound/RPG_Essentials_Free/10_Battle_SFX/69_Enemy_death_01.wav");
    soundMap[Hover] = LoadSound("../assets/sound/RPG_Essentials_Free/10_UI_Menu_SFX/001_Hover_01.wav");
    soundMap[Confirm] = LoadSound("../assets/sound/RPG_Essentials_Free/10_UI_Menu_SFX/013_Confirm_03.wav");
    soundMap[Pause] = LoadSound("../assets/sound/RPG_Essentials_Free/10_UI_Menu_SFX/092_Pause_04.wav");
    soundMap[Unpause] = LoadSound("../assets/sound/RPG_Essentials_Free/10_UI_Menu_SFX/098_Unpause_04.wav");
  }
};

#endif
