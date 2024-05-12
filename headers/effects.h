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

enum EffectType { Blood, Impact, Slash, Crystal, Capture };

class EffectMap {
public:
  static map<EffectType, Texture2D> effectMap;
  EffectMap() {}
  static void Init() {
    effectMap[Blood] = LoadTexture("../assets/effects/BloodSheet.png");
    effectMap[Impact] = LoadTexture("../assets/effects/Impact.png");
    effectMap[Slash] = LoadTexture("../assets/effects/slash.png");
    effectMap[Crystal] = LoadTexture("../assets/effects/Crystal.png");
    effectMap[Capture] = LoadTexture("../assets/effects/Capture.png");
  }
};

class Effect {
  int currentSpriteFrame;
  int maxFrames;
  int FrameCount;
  int FrameSpeed;
  bool Centered;
  Vector2 *position;
  EffectType effect;

public:
  Effect(EffectType effect, Vector2 *position, int speed, bool Centered = false)
      : effect(effect), position(position), Centered(Centered) {
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
    Vector2 pos = *position;
    float height = EffectMap::effectMap[effect].height;
    if (Centered) {
      pos.x -= height / 2;
      pos.y -= height / 2;
    }

    DrawTextureRec(
        EffectMap::effectMap[effect],
        Rectangle{(float)currentSpriteFrame * height, 0, height, height}, pos,
        WHITE);
    // If you can no longer draw the effect return true
    return currentSpriteFrame == maxFrames - 1;
  }
};

class EffectManager {
  static list<Effect> effectList; // Renamed from 'list' to 'effectList' to
                                  // avoid conflicts with the STL list

public:
  static void addEffect(EffectType effect, Vector2 *position, int speed, bool Capture = false) {
    Effect e(effect, position, speed, Capture);
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
