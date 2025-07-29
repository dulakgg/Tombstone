#include <Geode/Geode.hpp>
#include "DeathAnimations.hpp"

using namespace geode::prelude;

/*
===============================================================================================
                              THOMBSTONE DEATH ANIMATIONS SYSTEM
===============================================================================================

HOW TO ADD YOUR OWN ANIMATION:

1. CREATE YOUR ANIMATION FUNCTION:
   - Follow the naming pattern: void DeathAnimations::createYourNameAnimation(PlayLayer* playLayer, CCPoint playerPos)
   - Add detailed header comment with description, duration, style, and effects
   - Use the same function signature as existing animations
   - Place your function before the ANIMATION SELECTOR section

2. ADD TO HEADER FILE (DeathAnimations.hpp):
   - Add static function declaration: static void createYourNameAnimation(PlayLayer* playLayer, CCPoint playerPos);

3. UPDATE MOD SETTINGS (mod.json):
   - Add your animation name to the "one-of" array in "animation-type" setting
   - Example: "one-of": ["explosion", "ascension", "slaughterhouse", "yourname"]

4. UPDATE SELECTOR FUNCTION:
   - Add your condition in createSelectedAnimation() function
   - Example: else if (animationType == "yourname") { createYourNameAnimation(playLayer, playerPos); }

5. ANIMATION BEST PRACTICES:
   - Use CCSequence::create() for sequential actions
   - Use CCSpawn::create() for simultaneous actions  
   - Always call CCRemoveSelf::create() at the end to clean up sprites
   - Set appropriate Z-order values (higher = front layer)
   - Use log::info() for debugging with descriptive messages
   - Respect the duration setting from mod configuration

AVAILABLE COCOS2D ACTIONS:
- Movement: CCMoveTo, CCMoveBy, CCJumpTo, CCJumpBy
- Scaling: CCScaleTo, CCScaleBy
- Rotation: CCRotateTo, CCRotateBy
- Color: CCTintTo, CCTintBy
- Opacity: CCFadeIn, CCFadeOut, CCFadeTo
- Timing: CCDelayTime, CCRepeat
- Easing: CCEaseIn, CCEaseOut, CCEaseInOut
- Utility: CCRemoveSelf, CCCallFunc

===============================================================================================
*/

// ===============================================================================================
// ANIMATION 1: TELEPORTATION EXPLOSION - Interdimensional chaos with reality collapse

void DeathAnimations::createExplosionAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    log::info("🎬 EXPLOSION ANIMATION - Epic death sequence at position ({}, {})", playerPos.x, playerPos.y);
    
    auto player1 = playLayer->m_player1;
    auto player2 = playLayer->m_player2;
    auto mainPlayer = player1 ? player1 : player2;
    
    if (!mainPlayer) {
        log::warn("No player found for explosion animation");
        return;
    }
    
    if (mainPlayer) {
        mainPlayer->stopAllActions();
        mainPlayer->setColor(ccc3(255, 100, 100));
        mainPlayer->setScale(1.0f);
        
        mainPlayer->runAction(CCSequence::create(
            CCRepeat::create(
                CCSequence::create(
                    CCScaleTo::create(0.06f, 1.4f),
                    CCScaleTo::create(0.06f, 0.9f),
                    nullptr
                ), 6
            ),
            CCDelayTime::create(0.2f),
            CCSpawn::create(
                CCEaseOut::create(CCMoveBy::create(0.6f, ccp(-200, 150)), 3.0f),
                CCEaseInOut::create(CCScaleTo::create(0.6f, 2.2f), 2.0f),
                CCTintTo::create(0.6f, 100, 255, 255),
                nullptr
            ),
            CCSpawn::createWithTwoActions(
                CCFadeOut::create(0.08f),
                CCScaleTo::create(0.08f, 0.1f)
            ),
            CCDelayTime::create(0.25f),
            CCSpawn::create(
                CCFadeIn::create(0.15f),
                CCScaleTo::create(0.15f, 1.8f),
                CCMoveTo::create(0.15f, CCPoint(playerPos.x + 300, playerPos.y + 80)),
                CCTintTo::create(0.15f, 255, 255, 100),
                nullptr
            ),
            CCDelayTime::create(0.3f),
            CCSpawn::create(
                CCFadeOut::create(0.1f),
                CCScaleTo::create(0.1f, 0.05f),
                nullptr
            ),
            CCDelayTime::create(0.2f),
            CCSpawn::create(
                CCFadeIn::create(0.18f),
                CCScaleTo::create(0.18f, 2.8f),
                CCMoveTo::create(0.18f, CCPoint(playerPos.x - 150, playerPos.y - 100)),
                CCTintTo::create(0.18f, 255, 100, 255),
                nullptr
            ),
            CCDelayTime::create(0.35f),
            CCSpawn::create(
                CCFadeOut::create(0.08f),
                CCScaleTo::create(0.08f, 0.02f),
                nullptr
            ),
            CCDelayTime::create(0.15f),
            CCSpawn::create(
                CCFadeIn::create(0.2f),
                CCScaleTo::create(0.2f, 3.5f),
                CCMoveTo::create(0.2f, playerPos),
                CCTintTo::create(0.2f, 255, 255, 255),
                nullptr
            ),
            CCRepeat::create(
                CCSequence::create(
                    CCScaleTo::create(0.04f, 4.0f),
                    CCScaleTo::create(0.04f, 3.0f),
                    nullptr
                ), 8
            ),
            CCSpawn::create(
                CCEaseIn::create(CCScaleTo::create(0.8f, 0.0f), 4.0f),
                CCSequence::create(
                    CCTintTo::create(0.25f, 255, 0, 0),
                    CCTintTo::create(0.25f, 0, 255, 0),
                    CCTintTo::create(0.3f, 0, 0, 255),
                    nullptr
                ),
                CCSequence::create(
                    CCDelayTime::create(0.5f),
                    CCFadeOut::create(0.3f),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
    
    for (int portal = 0; portal < 4; portal++) {
        auto teleportPortal = CCSprite::create("GJ_square01.png");
        if (teleportPortal) {
            CCPoint portalPositions[4] = {
                CCPoint(playerPos.x - 200, playerPos.y + 150),
                CCPoint(playerPos.x + 300, playerPos.y + 80),
                CCPoint(playerPos.x - 150, playerPos.y - 100),
                playerPos
            };
            
            teleportPortal->setPosition(portalPositions[portal]);
            teleportPortal->setScale(0.0f);
            teleportPortal->setColor(ccc3(100 + portal * 50, 255 - portal * 40, 255));
            teleportPortal->setOpacity(0);
            teleportPortal->setZOrder(2800 + portal * 10);
            
            teleportPortal->runAction(CCSequence::create(
                CCDelayTime::create(1.0f + portal * 0.8f),
                CCSpawn::create(
                    CCFadeIn::create(0.2f),
                    CCEaseOut::create(CCScaleTo::create(0.2f, 4.0f + portal * 0.8f), 2.0f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCScaleTo::create(0.08f, 5.0f + portal * 0.8f),
                        CCScaleTo::create(0.08f, 3.5f + portal * 0.8f),
                        nullptr
                    ), 5
                ),
                CCSpawn::create(
                    CCFadeOut::create(0.25f),
                    CCScaleTo::create(0.25f, 0.0f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(teleportPortal);
        }
    }
    
    for (int timeWave = 0; timeWave < 6; timeWave++) {
        auto timeCrack = CCSprite::create("GJ_square01.png");
        if (timeCrack) {
            timeCrack->setPosition(CCPoint(
                playerPos.x + (rand() % 400 - 200),
                playerPos.y + (rand() % 300 - 150)
            ));
            timeCrack->setScale(0.0f);
            timeCrack->setColor(ccc3(255, 255 - timeWave * 30, 255 - timeWave * 40));
            timeCrack->setOpacity(200);
            timeCrack->setZOrder(2600);
            timeCrack->setRotation(rand() % 360);
            
            timeCrack->runAction(CCSequence::create(
                CCDelayTime::create(1.5f + timeWave * 0.2f),
                CCSpawn::create(
                    CCFadeIn::create(0.15f),
                    CCEaseOut::create(CCScaleTo::create(0.6f, 8.0f), 3.0f),
                    CCRotateBy::create(0.6f, 180 + rand() % 360),
                    nullptr
                ),
                CCDelayTime::create(0.3f),
                CCSpawn::create(
                    CCFadeOut::create(0.4f),
                    CCScaleTo::create(0.4f, 0.0f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(timeCrack);
        }
    }
    
    for (int fragment = 0; fragment < 25; fragment++) {
        auto realityFragment = CCSprite::create("GJ_square01.png");
        if (realityFragment) {
            realityFragment->setPosition(CCPoint(
                playerPos.x + (rand() % 500 - 250),
                playerPos.y + (rand() % 400 - 200)
            ));
            realityFragment->setScale(0.1f + (rand() % 80) / 100.0f);
            realityFragment->setColor(ccc3(255, 50 + rand() % 205, 100 + rand() % 155));
            realityFragment->setZOrder(2400);
            realityFragment->setOpacity(0);
            realityFragment->setRotation(rand() % 360);
            
            float glitchDirection = (rand() % 2 == 0) ? 1.0f : -1.0f;
            
            realityFragment->runAction(CCSequence::create(
                CCDelayTime::create(0.8f + (rand() % 120) / 100.0f),
                CCSpawn::create(
                    CCFadeIn::create(0.1f),
                    CCScaleTo::create(0.1f, 1.2f + (rand() % 60) / 100.0f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCMoveBy::create(0.06f, ccp(glitchDirection * 20, 0)),
                        CCMoveBy::create(0.06f, ccp(-glitchDirection * 20, 0)),
                        CCMoveBy::create(0.06f, ccp(0, glitchDirection * 15)),
                        CCMoveBy::create(0.06f, ccp(0, -glitchDirection * 15)),
                        nullptr
                    ), 8
                ),
                CCSpawn::create(
                    CCEaseIn::create(CCMoveBy::create(1.0f, ccp(
                        glitchDirection * (200 + rand() % 300),
                        (rand() % 400 - 200)
                    )), 3.0f),
                    CCRotateBy::create(1.0f, glitchDirection * (720 + rand() % 1080)),
                    CCSequence::create(
                        CCDelayTime::create(0.5f),
                        CCFadeOut::create(0.5f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(realityFragment);
        }
    }
    
    for (int vortex = 0; vortex < 3; vortex++) {
        auto dimensionVortex = CCSprite::create("GJ_square01.png");
        if (dimensionVortex) {
            CCPoint vortexPositions[3] = {
                CCPoint(playerPos.x - 180, playerPos.y + 120),
                CCPoint(playerPos.x + 250, playerPos.y + 60),
                CCPoint(playerPos.x - 100, playerPos.y - 80)
            };
            
            dimensionVortex->setPosition(vortexPositions[vortex]);
            dimensionVortex->setScale(0.0f);
            dimensionVortex->setColor(ccc3(100 + vortex * 77, 50, 255 - vortex * 50));
            dimensionVortex->setOpacity(150);
            dimensionVortex->setZOrder(2200);
            
            dimensionVortex->runAction(CCSequence::create(
                CCDelayTime::create(1.2f + vortex * 0.6f),
                CCSpawn::create(
                    CCFadeIn::create(0.3f),
                    CCEaseOut::create(CCScaleTo::create(0.8f, 6.0f + vortex * 2.0f), 2.5f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCRotateBy::create(0.15f, 60),
                        CCScaleTo::create(0.08f, 7.0f + vortex * 2.0f),
                        CCScaleTo::create(0.08f, 5.5f + vortex * 2.0f),
                        nullptr
                    ), 6
                ),
                CCSpawn::create(
                    CCFadeOut::create(0.5f),
                    CCScaleTo::create(0.5f, 0.0f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(dimensionVortex);
        }
    }
    
    for (int wave = 0; wave < 10; wave++) {
        auto realityCollapseWave = CCSprite::create("GJ_square01.png");
        if (realityCollapseWave) {
            realityCollapseWave->setPosition(playerPos);
            realityCollapseWave->setScale(0.1f);
            realityCollapseWave->setColor(ccc3(255 - wave * 20, 100 + wave * 15, 255));
            realityCollapseWave->setOpacity(180 - wave * 15);
            realityCollapseWave->setZOrder(1900 - wave * 8);
            
            realityCollapseWave->runAction(CCSequence::create(
                CCDelayTime::create(3.5f + wave * 0.05f),
                CCSpawn::create(
                    CCEaseOut::create(CCScaleTo::create(1.2f, 15.0f + wave * 4), 3.5f),
                    CCSequence::create(
                        CCDelayTime::create(0.4f),
                        CCFadeOut::create(0.8f),
                        nullptr
                    ),
                    CCSequence::create(
                        CCTintTo::create(0.2f, 255, 255, 255),
                        CCTintTo::create(0.2f, 100, 100, 255),
                        CCTintTo::create(0.2f, 255, 100, 100),
                        CCTintTo::create(0.6f, 255, 255, 255),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(realityCollapseWave);
        }
    }
    
    for (int i = 0; i < 60; i++) {
        auto particle = CCSprite::create("GJ_square01.png");
        if (particle) {
            particle->setPosition(playerPos);
            particle->setScale(0.2f + (rand() % 50) / 100.0f);
            particle->setColor(ccc3(255, 100 + rand() % 155, 20 + rand() % 100));
            particle->setZOrder(1800);
            
            float angle = (rand() % 360) * M_PI / 180.0f;
            float speed = 100 + rand() % 200;
            CCPoint targetPos = CCPoint(
                playerPos.x + cos(angle) * speed,
                playerPos.y + sin(angle) * speed
            );
            
            particle->runAction(CCSequence::create(
                CCDelayTime::create(2.5f + (rand() % 40) / 100.0f),
                CCSpawn::create(
                    CCEaseOut::create(CCMoveTo::create(1.8f, targetPos), 2.5f),
                    CCRotateBy::create(1.8f, 540 + rand() % 720),
                    CCSequence::create(
                        CCDelayTime::create(0.8f),
                        CCFadeOut::create(1.0f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(particle);
        }
    }
    
    for (int i = 0; i < 30; i++) {
        auto sparkle = CCSprite::create("GJ_square01.png");
        if (sparkle) {
            sparkle->setPosition(CCPoint(
                playerPos.x + (rand() % 300 - 150),
                playerPos.y + (rand() % 150 - 75)
            ));
            sparkle->setScale(0.05f);
            sparkle->setColor(ccc3(255, 255, 100 + rand() % 155));
            sparkle->setZOrder(1700);
            
            sparkle->runAction(CCSequence::create(
                CCDelayTime::create(4.0f + (rand() % 80) / 100.0f),
                CCSpawn::createWithTwoActions(
                    CCScaleTo::create(0.4f, 0.4f),
                    CCFadeIn::create(0.4f)
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCScaleTo::create(0.1f, 0.5f),
                        CCScaleTo::create(0.1f, 0.3f),
                        nullptr
                    ), 4
                ),
                CCSpawn::createWithTwoActions(
                    CCFadeOut::create(0.6f),
                    CCScaleTo::create(0.6f, 0.0f)
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(sparkle);
        }
    }
    
    auto finalZoom = CCSprite::create("GJ_square01.png");
    if (finalZoom) {
        finalZoom->setPosition(playerPos);
        finalZoom->setScale(0.0f);
        finalZoom->setColor(ccc3(100, 50, 200));
        finalZoom->setOpacity(100);
        finalZoom->setZOrder(1600);
        
        finalZoom->runAction(CCSequence::create(
            CCDelayTime::create(4.8f),
            CCSpawn::createWithTwoActions(
                CCEaseOut::create(CCScaleTo::create(0.2f, 20.0f), 4.0f),
                CCFadeOut::create(0.2f)
            ),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(finalZoom);
    }
}

// ===============================================================================================
// ANIMATION 2: HEAVENLY ASCENSION - Divine ascension with dramatic fall

void DeathAnimations::createAscensionAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    log::info("✨ ASCENSION ANIMATION - Epic rise and fall sequence at position ({}, {})", playerPos.x, playerPos.y);
    
    auto player1 = playLayer->m_player1;
    auto player2 = playLayer->m_player2;
    auto mainPlayer = player1 ? player1 : player2;
    
    if (!mainPlayer) {
        log::warn("No player found for ascension animation");
        return;
    }
    
    if (mainPlayer) {
        mainPlayer->stopAllActions();
        mainPlayer->setColor(ccc3(255, 255, 200));
        mainPlayer->setScale(1.0f);
        
        mainPlayer->runAction(CCSequence::create(
            CCSpawn::create(
                CCEaseInOut::create(CCMoveBy::create(0.8f, ccp(0, 20)), 2.0f),
                CCSequence::create(
                    CCScaleTo::create(0.4f, 1.3f),
                    CCScaleTo::create(0.4f, 1.0f),
                    nullptr
                ),
                CCTintTo::create(0.8f, 255, 255, 255),
                nullptr
            ),
            CCDelayTime::create(0.2f),
            CCSpawn::create(
                CCEaseOut::create(CCMoveBy::create(2.5f, ccp(0, 400)), 2.0f),
                CCRotateBy::create(2.5f, 360.0f),
                CCSequence::create(
                    CCScaleTo::create(1.0f, 1.8f),
                    CCScaleTo::create(1.5f, 1.2f),
                    nullptr
                ),
                CCSequence::create(
                    CCTintTo::create(1.0f, 255, 255, 100),
                    CCTintTo::create(1.5f, 255, 255, 255),
                    nullptr
                ),
                nullptr
            ),
            CCDelayTime::create(0.3f),
            CCSpawn::create(
                CCEaseIn::create(CCMoveBy::create(1.5f, ccp(0, -500)), 4.0f),
                CCRotateBy::create(1.5f, 1440.0f),
                CCSequence::create(
                    CCScaleTo::create(0.5f, 0.8f),
                    CCScaleTo::create(0.5f, 1.5f),
                    CCScaleTo::create(0.5f, 0.3f),
                    nullptr
                ),
                CCSequence::create(
                    CCTintTo::create(0.3f, 255, 100, 100),
                    CCTintTo::create(0.6f, 100, 100, 255),
                    CCTintTo::create(0.6f, 255, 255, 255),
                    nullptr
                ),
                nullptr
            ),
            CCSpawn::createWithTwoActions(
                CCScaleTo::create(0.2f, 0.0f),
                CCFadeOut::create(0.2f)
            ),
            nullptr
        ));
    }
    
    for (int wing = 0; wing < 2; wing++) {
        for (int feather = 0; feather < 8; feather++) {
            auto wingFeather = CCSprite::create("GJ_square01.png");
            if (wingFeather) {
                float side = wing == 0 ? -1.0f : 1.0f;
                float wingAngle = side * (20 + feather * 12);
                float distance = 50 + feather * 15;
                
                CCPoint featherPos = CCPoint(
                    playerPos.x + side * distance * cos(wingAngle * M_PI / 180.0f),
                    playerPos.y + distance * sin(wingAngle * M_PI / 180.0f)
                );
                
                wingFeather->setPosition(featherPos);
                wingFeather->setScale(0.0f);
                wingFeather->setColor(ccc3(255, 255, 200 + feather * 5));
                wingFeather->setOpacity(200);
                wingFeather->setZOrder(2700);
                wingFeather->setRotation(wingAngle);
                
                wingFeather->runAction(CCSequence::create(
                    CCDelayTime::create(0.5f + feather * 0.08f),
                    CCSpawn::create(
                        CCScaleTo::create(0.4f, 2.0f + feather * 0.3f),
                        CCFadeIn::create(0.4f),
                        CCRotateBy::create(0.4f, side * 30),
                        nullptr
                    ),
                    CCSpawn::create(
                        CCMoveBy::create(2.0f, ccp(0, 200 + feather * 20)),
                        CCRotateBy::create(2.0f, side * 180),
                        nullptr
                    ),
                    CCSpawn::create(
                        CCMoveBy::create(1.5f, ccp(side * 100, -300)),
                        CCRotateBy::create(1.5f, side * 360),
                        CCFadeOut::create(1.5f),
                        nullptr
                    ),
                    CCRemoveSelf::create(),
                    nullptr
                ));
                
                playLayer->addChild(wingFeather);
            }
        }
    }
    
    auto lightPillar = CCSprite::create("GJ_square01.png");
    if (lightPillar) {
        auto winSize = CCDirector::get()->getWinSize();
        lightPillar->setPosition(CCPoint(playerPos.x, winSize.height / 2));
        lightPillar->setScaleX(3.0f);
        lightPillar->setScaleY(winSize.height / lightPillar->getContentSize().height);
        lightPillar->setColor(ccc3(255, 255, 150));
        lightPillar->setOpacity(0);
        lightPillar->setZOrder(2600);
        
        lightPillar->runAction(CCSequence::create(
            CCDelayTime::create(1.0f),
            CCFadeTo::create(0.8f, 150),
            CCDelayTime::create(2.0f),
            CCFadeOut::create(1.2f),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(lightPillar);
    }
    
    for (int i = 0; i < 20; i++) {
        auto musicalNote = CCSprite::create("GJ_square01.png");
        if (musicalNote) {
            musicalNote->setPosition(CCPoint(
                playerPos.x + (rand() % 400 - 200),
                playerPos.y + (rand() % 150 - 75)
            ));
            musicalNote->setScale(0.1f + (rand() % 40) / 100.0f);
            musicalNote->setColor(ccc3(255, 255, 150 + rand() % 105));
            musicalNote->setZOrder(2500);
            musicalNote->setOpacity(0);
            
            float floatDirection = (rand() % 2 == 0) ? 1.0f : -1.0f;
            
            musicalNote->runAction(CCSequence::create(
                CCDelayTime::create(1.5f + (rand() % 150) / 100.0f),
                CCFadeIn::create(0.4f),
                CCSpawn::create(
                    CCEaseInOut::create(CCMoveBy::create(2.5f, ccp(floatDirection * 80, 250)), 2.0f),
                    CCRotateBy::create(2.5f, floatDirection * 360),
                    CCSequence::create(
                        CCDelayTime::create(1.5f),
                        CCFadeOut::create(1.0f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(musicalNote);
        }
    }
    
    for (int halo = 0; halo < 5; halo++) {
        auto angelHalo = CCSprite::create("GJ_square01.png");
        if (angelHalo) {
            angelHalo->setPosition(CCPoint(playerPos.x, playerPos.y - 80));
            angelHalo->setScale(0.1f);
            angelHalo->setColor(ccc3(255, 255, 80 + halo * 20));
            angelHalo->setOpacity(180);
            angelHalo->setZOrder(2400);
            
            angelHalo->runAction(CCSequence::create(
                CCDelayTime::create(2.0f + halo * 0.2f),
                CCSpawn::create(
                    CCEaseOut::create(CCScaleTo::create(2.0f, 4.0f + halo * 0.8f), 2.0f),
                    CCEaseOut::create(CCMoveBy::create(2.0f, ccp(0, 300 + halo * 80)), 2.0f),
                    CCSequence::create(
                        CCDelayTime::create(1.0f),
                        CCFadeOut::create(1.0f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(angelHalo);
        }
    }
    
    for (int i = 0; i < 40; i++) {
        auto blessingStar = CCSprite::create("GJ_square01.png");
        if (blessingStar) {
            blessingStar->setPosition(CCPoint(
                playerPos.x + (rand() % 600 - 300),
                playerPos.y + (rand() % 300 - 150)
            ));
            blessingStar->setScale(0.05f);
            blessingStar->setColor(ccc3(255, 255, 200 + rand() % 55));
            blessingStar->setZOrder(2300);
            blessingStar->setOpacity(0);
            
            blessingStar->runAction(CCSequence::create(
                CCDelayTime::create(3.5f + (rand() % 120) / 100.0f),
                CCSpawn::create(
                    CCFadeIn::create(0.3f),
                    CCScaleTo::create(0.3f, 0.6f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCScaleTo::create(0.15f, 0.8f),
                        CCScaleTo::create(0.15f, 0.4f),
                        nullptr
                    ), 4
                ),
                CCSpawn::createWithTwoActions(
                    CCFadeOut::create(0.8f),
                    CCScaleTo::create(0.8f, 0.0f)
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(blessingStar);
        }
    }
    
    log::info("👼 ASCENSION COMPLETE: Player ascended to heaven and fell dramatically!");
}

// ===============================================================================================
// ANIMATION 3: SLAUGHTERHOUSE BRUTALITY - Ultra-violent, cinematic death sequence
// Duration: 3 seconds | Style: Slaughterhouse-level intensity, maximum violence and drama

void DeathAnimations::createSlaughterhouseAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    log::info("💀 SLAUGHTERHOUSE ANIMATION - BRUTAL death sequence at position ({}, {})", playerPos.x, playerPos.y);
    
    auto player1 = playLayer->m_player1;
    auto player2 = playLayer->m_player2;
    auto mainPlayer = player1 ? player1 : player2;
    
    if (!mainPlayer) {
        log::warn("No player found for slaughterhouse animation");
        return;
    }
    
    if (mainPlayer) {
        mainPlayer->stopAllActions();
        mainPlayer->setColor(ccc3(255, 255, 255));
        mainPlayer->setScale(1.0f);
        
        mainPlayer->runAction(CCSequence::create(
            CCRepeat::create(
                CCSequence::create(
                    CCMoveBy::create(0.02f, ccp(5, 0)),
                    CCMoveBy::create(0.02f, ccp(-10, 0)),
                    CCMoveBy::create(0.02f, ccp(5, 0)),
                    nullptr
                ), 8
            ),
            CCSpawn::create(
                CCTintTo::create(0.1f, 255, 0, 0),
                CCScaleTo::create(0.1f, 1.8f),
                nullptr
            ),
            CCRepeat::create(
                CCSequence::create(
                    CCMoveBy::create(0.01f, ccp(15, 0)),
                    CCMoveBy::create(0.01f, ccp(-15, 0)),
                    CCMoveBy::create(0.01f, ccp(0, 10)),
                    CCMoveBy::create(0.01f, ccp(0, -10)),
                    nullptr
                ), 20
            ),
            CCSpawn::create(
                CCEaseIn::create(CCScaleTo::create(0.3f, 3.5f), 4.0f),
                CCTintTo::create(0.3f, 150, 0, 0),
                CCRotateBy::create(0.3f, 180),
                nullptr
            ),
            CCRepeat::create(
                CCSequence::create(
                    CCMoveBy::create(0.005f, ccp(25, 0)),
                    CCMoveBy::create(0.005f, ccp(-25, 0)),
                    CCMoveBy::create(0.005f, ccp(0, 20)),
                    CCMoveBy::create(0.005f, ccp(0, -20)),
                    nullptr
                ), 40
            ),
            CCSpawn::create(
                CCEaseIn::create(CCScaleTo::create(0.4f, 0.1f), 5.0f),
                CCTintTo::create(0.4f, 100, 0, 0),
                CCRotateBy::create(0.4f, 720),
                CCSequence::create(
                    CCDelayTime::create(0.2f),
                    CCFadeOut::create(0.2f),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
    
    for (int splatter = 0; splatter < 40; splatter++) {
        auto bloodSplatter = CCSprite::create("GJ_square01.png");
        if (bloodSplatter) {
            bloodSplatter->setPosition(playerPos);
            bloodSplatter->setScale(0.3f + (rand() % 100) / 100.0f);
            bloodSplatter->setColor(ccc3(200 + rand() % 55, rand() % 50, rand() % 30));
            bloodSplatter->setZOrder(2900);
            bloodSplatter->setOpacity(255);
            
            float explosionAngle = (rand() % 360) * M_PI / 180.0f;
            float explosionSpeed = 200 + rand() % 400;
            CCPoint targetPos = CCPoint(
                playerPos.x + cos(explosionAngle) * explosionSpeed,
                playerPos.y + sin(explosionAngle) * explosionSpeed
            );
            
            bloodSplatter->runAction(CCSequence::create(
                CCDelayTime::create(0.5f + (rand() % 20) / 100.0f),
                CCSpawn::create(
                    CCEaseOut::create(CCMoveTo::create(0.8f, targetPos), 3.0f),
                    CCRotateBy::create(0.8f, 1080 + rand() % 1440),
                    CCSequence::create(
                        CCScaleTo::create(0.2f, 1.5f + (rand() % 100) / 100.0f),
                        CCScaleTo::create(0.6f, 0.2f),
                        nullptr
                    ),
                    CCSequence::create(
                        CCDelayTime::create(0.4f),
                        CCFadeOut::create(0.4f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(bloodSplatter);
        }
    }
    
    for (int gore = 0; gore < 15; gore++) {
        auto goreChunk = CCSprite::create("GJ_square01.png");
        if (goreChunk) {
            goreChunk->setPosition(CCPoint(
                playerPos.x + (rand() % 60 - 30),
                playerPos.y + (rand() % 60 - 30)
            ));
            goreChunk->setScale(0.8f + (rand() % 80) / 100.0f);
            goreChunk->setColor(ccc3(180 + rand() % 75, rand() % 30, rand() % 20));
            goreChunk->setZOrder(2800);
            goreChunk->setOpacity(255);
            
            float chunkAngle = (rand() % 360) * M_PI / 180.0f;
            float chunkSpeed = 150 + rand() % 300;
            CCPoint chunkTarget = CCPoint(
                playerPos.x + cos(chunkAngle) * chunkSpeed,
                playerPos.y + sin(chunkAngle) * chunkSpeed - 100
            );
            
            goreChunk->runAction(CCSequence::create(
                CCDelayTime::create(0.4f + gore * 0.02f),
                CCSpawn::create(
                    CCJumpTo::create(1.2f, chunkTarget, 80 + rand() % 120, 1),
                    CCRotateBy::create(1.2f, 720 + rand() % 1080),
                    CCSequence::create(
                        CCDelayTime::create(0.6f),
                        CCFadeOut::create(0.6f),
                        nullptr
                    ),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(goreChunk);
        }
    }
    
    auto bloodOverlay = CCLayerColor::create(ccc4(150, 0, 0, 0));
    bloodOverlay->setContentSize(CCDirector::get()->getWinSize());
    bloodOverlay->setPosition(CCPointZero);
    bloodOverlay->setZOrder(2700);
    bloodOverlay->runAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCFadeTo::create(0.1f, 180),
        CCRepeat::create(
            CCSequence::create(
                CCFadeTo::create(0.05f, 220),
                CCFadeTo::create(0.05f, 140),
                nullptr
            ), 15
        ),
        CCFadeTo::create(0.8f, 0),
        CCRemoveSelf::create(),
        nullptr
    ));
    playLayer->addChild(bloodOverlay);
    
    for (int shockwave = 0; shockwave < 8; shockwave++) {
        auto violentShockwave = CCSprite::create("GJ_square01.png");
        if (violentShockwave) {
            violentShockwave->setPosition(playerPos);
            violentShockwave->setScale(0.2f);
            violentShockwave->setColor(ccc3(255, 50, 50));
            violentShockwave->setOpacity(200);
            violentShockwave->setZOrder(2600 - shockwave * 5);
            
            violentShockwave->runAction(CCSequence::create(
                CCDelayTime::create(0.6f + shockwave * 0.08f),
                CCSpawn::create(
                    CCEaseOut::create(CCScaleTo::create(0.5f, 20.0f + shockwave * 8), 4.0f),
                    CCSequence::create(
                        CCTintTo::create(0.1f, 255, 0, 0),
                        CCTintTo::create(0.1f, 100, 0, 0),
                        CCTintTo::create(0.3f, 50, 0, 0),
                        nullptr
                    ),
                    CCFadeOut::create(0.5f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(violentShockwave);
        }
    }
    
    for (int distortion = 0; distortion < 20; distortion++) {
        auto screenDistortion = CCSprite::create("GJ_square01.png");
        if (screenDistortion) {
            screenDistortion->setPosition(CCPoint(
                playerPos.x + (rand() % 600 - 300),
                playerPos.y + (rand() % 400 - 200)
            ));
            screenDistortion->setScale(0.0f);
            screenDistortion->setColor(ccc3(255, rand() % 100, rand() % 100));
            screenDistortion->setZOrder(2500);
            screenDistortion->setOpacity(150);
            
            screenDistortion->runAction(CCSequence::create(
                CCDelayTime::create(0.8f + (rand() % 40) / 100.0f),
                CCSpawn::create(
                    CCScaleTo::create(0.1f, 3.0f + (rand() % 200) / 100.0f),
                    CCFadeIn::create(0.1f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCScaleTo::create(0.02f, 4.0f),
                        CCScaleTo::create(0.02f, 2.0f),
                        nullptr
                    ), 25
                ),
                CCSpawn::create(
                    CCFadeOut::create(0.3f),
                    CCScaleTo::create(0.3f, 0.0f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(screenDistortion);
        }
    }
    
    auto finalCarnage = CCSprite::create("GJ_square01.png");
    if (finalCarnage) {
        finalCarnage->setPosition(playerPos);
        finalCarnage->setScale(0.0f);
        finalCarnage->setColor(ccc3(100, 0, 0));
        finalCarnage->setOpacity(200);
        finalCarnage->setZOrder(2400);
        
        finalCarnage->runAction(CCSequence::create(
            CCDelayTime::create(2.5f),
            CCSpawn::create(
                CCEaseOut::create(CCScaleTo::create(0.5f, 25.0f), 5.0f),
                CCSequence::create(
                    CCTintTo::create(0.1f, 200, 0, 0),
                    CCTintTo::create(0.1f, 50, 0, 0),
                    CCTintTo::create(0.3f, 0, 0, 0),
                    nullptr
                ),
                CCFadeOut::create(0.5f),
                nullptr
            ),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(finalCarnage);
    }
    
    log::info("💀 SLAUGHTERHOUSE COMPLETE: Player has been BRUTALLY destroyed!");
}
// ===============================================================================================
// ANIMATION SELECTOR - Choose which animation to play based on mod settings
// Add new animations here by:
// 1. Create your animation function above (follow the format)
// 2. Add the animation name to mod.json settings
// 3. Add the condition below to call your function
// ===============================================================================================

void DeathAnimations::createSelectedAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    std::string animationType = Mod::get()->getSettingValue<std::string>("animation-type");
    
    if (animationType == "ascension") {
        createAscensionAnimation(playLayer, playerPos);
    } else if (animationType == "slaughterhouse") {
        createSlaughterhouseAnimation(playLayer, playerPos);
    } else {
        createExplosionAnimation(playLayer, playerPos);
    }
}
