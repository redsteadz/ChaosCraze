#ifndef EFFECT_H
#define EFFECT_H

#include "raylib.h"
#include <bits/stdc++.h>
using namespace std;
// Make an Effects Class that holds information about the
// 1. Effect texture (Enum of effects to choose from a map)
// 2. currentFrame
// 3. FrameCount
// 4. FrameSpeed

enum EffectType { Blood, Impact, Slash };

class EffectMap {
public:
  static map<EffectType, Texture2D> effectMap;
  EffectMap() {}
  static void Init() {
    effectMap[Blood] = LoadTexture("../assets/effects/BloodSheet.png");
    effectMap[Impact] = LoadTexture("../assets/effects/Impact.png");
    effectMap[Slash] = LoadTexture("../assets/effects/slash.png");
  }
};

class Effect {
  int currentSpriteFrame;
  int maxFrames;
  int FrameCount;
  int FrameSpeed;
  Vector2 position;
  EffectType effect;

public:
  Effect(EffectType effect, Vector2 position, int speed)
      : effect(effect), position(position) {
    currentSpriteFrame = 0;
    maxFrames = EffectMap::effectMap[effect].width /
                EffectMap::effectMap[effect].height;
    FrameCount = 0;
    FrameSpeed = speed;
  }
  void Update() {
    if (FrameCount >= (60 / FrameSpeed)) {
      currentSpriteFrame = (currentSpriteFrame + 1);
      FrameCount = 0;
    }
    FrameCount++;
  }
  bool Draw() {
    DrawTextureRec(EffectMap::effectMap[effect],
                   Rectangle{(float)currentSpriteFrame * 32, 0, 32, 32},
                   position, WHITE);
    // If you can no longer draw the effect return true
    return currentSpriteFrame == maxFrames - 1;
  }
};

class EffectManager {
  static list<Effect> effectList; // Renamed from 'list' to 'effectList' to
                                  // avoid conflicts with the STL list

public:
  static void addEffect(EffectType effect, Vector2 position, int speed) {
    Effect e(effect, position, speed);
    effectList.push_back(
        e); // Using push_back() to add the effect to the end of the list
  }

  static void updateEffects() {
    for (auto it = effectList.begin(); it != effectList.end();) {
      Effect &e = *it;
      e.Update();
      if (e.Draw()) {
        it = effectList.erase(it);
      } else {
        ++it;
      }
    }
  }
};
#endif
