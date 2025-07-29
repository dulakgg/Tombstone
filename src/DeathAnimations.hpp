#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DeathAnimations {
public:
    static void createExplosionAnimation(PlayLayer* playLayer, CCPoint playerPos);
    static void createAscensionAnimation(PlayLayer* playLayer, CCPoint playerPos);
    static void createSlaughterhouseAnimation(PlayLayer* playLayer, CCPoint playerPos);
    static void createSelectedAnimation(PlayLayer* playLayer, CCPoint playerPos);
};
