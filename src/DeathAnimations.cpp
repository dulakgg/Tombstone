#include "DeathAnimations.hpp"
#include <Geode/modify/PlayLayer.hpp>

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
        mainPlayer->setColor(ccc3(255, 200, 200));
        
        mainPlayer->runAction(CCSequence::create(
            CCDelayTime::create(0.05f),
            CCRepeat::create(
                CCSequence::create(
                    CCScaleTo::create(0.1f, 1.2f),
                    CCScaleTo::create(0.1f, 1.0f),
                    nullptr
                ), 3
            ),
            nullptr
        ));
    }
    
    for (int i = 0; i < 8; i++) {
        auto energyOrb = CCSprite::create("GJ_square01.png");
        if (energyOrb) {
            energyOrb->setScale(0.0f);
            energyOrb->setColor(ccc3(100, 200, 255));
            energyOrb->setZOrder(2500);
            
            float angle = (360.0f / 8) * i;
            float radius = 60.0f;
            CCPoint orbPos = CCPoint(
                playerPos.x + cos(angle * M_PI / 180.0f) * radius,
                playerPos.y + sin(angle * M_PI / 180.0f) * radius
            );
            energyOrb->setPosition(orbPos);
            
            energyOrb->runAction(CCSequence::create(
                CCDelayTime::create(1.0f + i * 0.1f),
                CCScaleTo::create(0.2f, 0.5f),
                CCRepeat::create(
                    CCSequence::create(
                        CCMoveTo::create(0.125f, CCPoint(
                            playerPos.x + cos((angle + 90) * M_PI / 180.0f) * radius,
                            playerPos.y + sin((angle + 90) * M_PI / 180.0f) * radius
                        )),
                        CCMoveTo::create(0.125f, CCPoint(
                            playerPos.x + cos((angle + 180) * M_PI / 180.0f) * radius,
                            playerPos.y + sin((angle + 180) * M_PI / 180.0f) * radius
                        )),
                        CCMoveTo::create(0.125f, CCPoint(
                            playerPos.x + cos((angle + 270) * M_PI / 180.0f) * radius,
                            playerPos.y + sin((angle + 270) * M_PI / 180.0f) * radius
                        )),
                        CCMoveTo::create(0.125f, orbPos),
                        nullptr
                    ), 4
                ),
                CCSpawn::createWithTwoActions(
                    CCScaleTo::create(0.3f, 0.0f),
                    CCMoveTo::create(0.3f, playerPos)
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(energyOrb);
        }
    }
    
    auto darkOverlay = CCLayerColor::create(ccc4(0, 0, 0, 0));
    darkOverlay->setContentSize(CCDirector::get()->getWinSize());
    darkOverlay->setPosition(CCPointZero);
    darkOverlay->setZOrder(2000);
    darkOverlay->runAction(CCSequence::create(
        CCDelayTime::create(2.0f),
        CCFadeTo::create(0.3f, 180),
        CCDelayTime::create(1.0f),
        CCFadeTo::create(1.0f, 0),
        CCRemoveSelf::create(),
        nullptr
    ));
    playLayer->addChild(darkOverlay);
    
    if (mainPlayer) {
        mainPlayer->runAction(CCSequence::create(
            CCDelayTime::create(2.0f),
            CCSpawn::createWithTwoActions(
                CCTintTo::create(0.1f, 255, 255, 255),
                CCScaleTo::create(0.1f, 1.0f)
            ),
            CCSpawn::create(
                CCJumpBy::create(1.5f, ccp(0, 0), 150.0f, 1),
                CCRotateBy::create(1.5f, 720.0f),
                CCSequence::create(
                    CCScaleTo::create(0.3f, 1.5f),
                    CCScaleTo::create(1.2f, 0.8f),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
    
    for (int ring = 0; ring < 5; ring++) {
        auto explosionRing = CCSprite::create("GJ_square01.png");
        if (explosionRing) {
            explosionRing->setPosition(playerPos);
            explosionRing->setScale(0.1f);
            explosionRing->setColor(ccc3(255, 100 - ring * 15, 50));
            explosionRing->setOpacity(200);
            explosionRing->setZOrder(1900 - ring * 10);
            
            explosionRing->runAction(CCSequence::create(
                CCDelayTime::create(2.0f + ring * 0.1f),
                CCSpawn::createWithTwoActions(
                    CCEaseOut::create(CCScaleTo::create(1.5f, 8.0f + ring * 2), 2.0f),
                    CCFadeOut::create(1.5f)
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(explosionRing);
        }
    }
    
    for (int i = 0; i < 40; i++) {
        auto particle = CCSprite::create("GJ_square01.png");
        if (particle) {
            particle->setPosition(playerPos);
            particle->setScale(0.3f + (rand() % 40) / 100.0f);
            particle->setColor(ccc3(255, 150 + rand() % 105, 50 + rand() % 150));
            particle->setZOrder(1800);
            
            float angle = (rand() % 360) * M_PI / 180.0f;
            float speed = 80 + rand() % 120;
            CCPoint targetPos = CCPoint(
                playerPos.x + cos(angle) * speed,
                playerPos.y + sin(angle) * speed
            );
            
            particle->runAction(CCSequence::create(
                CCDelayTime::create(2.0f + (rand() % 30) / 100.0f),
                CCSpawn::create(
                    CCEaseOut::create(CCMoveTo::create(2.0f, targetPos), 2.0f),
                    CCRotateBy::create(2.0f, 360 + rand() % 720),
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
            
            playLayer->addChild(particle);
        }
    }
    
    if (mainPlayer) {
        mainPlayer->runAction(CCSequence::create(
            CCDelayTime::create(3.5f),
            CCSpawn::create(
                CCEaseIn::create(CCMoveBy::create(1.0f, ccp(0, -200)), 2.0f),
                CCSequence::create(
                    CCRotateBy::create(0.5f, 180.0f),
                    CCRotateBy::create(0.5f, 180.0f),
                    nullptr
                ),
                CCSequence::create(
                    CCFadeTo::create(0.8f, 100),
                    CCFadeTo::create(0.2f, 255),
                    nullptr
                ),
                nullptr
            ),
            CCSpawn::createWithTwoActions(
                CCScaleTo::create(0.3f, 0.0f),
                CCFadeOut::create(0.3f)
            ),
            nullptr
        ));
    }
    
    for (int i = 0; i < 20; i++) {
        auto sparkle = CCSprite::create("GJ_square01.png");
        if (sparkle) {
            sparkle->setPosition(CCPoint(
                playerPos.x + (rand() % 200 - 100),
                playerPos.y + (rand() % 100 - 50)
            ));
            sparkle->setScale(0.1f);
            sparkle->setColor(ccc3(255, 255, 150 + rand() % 105));
            sparkle->setZOrder(1700);
            
            sparkle->runAction(CCSequence::create(
                CCDelayTime::create(4.0f + (rand() % 50) / 100.0f),
                CCSpawn::createWithTwoActions(
                    CCScaleTo::create(0.5f, 0.3f),
                    CCFadeOut::create(1.0f)
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
        finalZoom->setColor(ccc3(50, 50, 150));
        finalZoom->setOpacity(80);
        finalZoom->setZOrder(1600);
        
        finalZoom->runAction(CCSequence::create(
            CCDelayTime::create(4.5f),
            CCSpawn::createWithTwoActions(
                CCEaseOut::create(CCScaleTo::create(0.5f, 15.0f), 3.0f),
                CCFadeOut::create(0.5f)
            ),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(finalZoom);
    }
}

void DeathAnimations::createAscensionAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    log::info("✨ ASCENSION ANIMATION - Heavenly death sequence at position ({}, {})", playerPos.x, playerPos.y);
    
    auto player1 = playLayer->m_player1;
    auto player2 = playLayer->m_player2;
    auto mainPlayer = player1 ? player1 : player2;
    
    if (!mainPlayer) {
        log::warn("No player found for ascension animation");
        return;
    }
    
    auto divineLightRing = CCSprite::create("GJ_square01.png");
    if (divineLightRing) {
        divineLightRing->setPosition(playerPos);
        divineLightRing->setScale(0.0f);
        divineLightRing->setColor(ccc3(255, 255, 200));
        divineLightRing->setOpacity(100);
        divineLightRing->setZOrder(2800);
        
        divineLightRing->runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCSpawn::createWithTwoActions(
                CCEaseOut::create(CCScaleTo::create(0.8f, 4.0f), 2.0f),
                CCFadeOut::create(0.8f)
            ),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(divineLightRing);
    }
    
    if (mainPlayer) {
        mainPlayer->stopAllActions();
        mainPlayer->setColor(ccc3(255, 255, 220));
        
        mainPlayer->runAction(CCSequence::create(
            CCRepeat::create(
                CCSequence::create(
                    CCEaseInOut::create(CCMoveBy::create(0.5f, ccp(0, 10)), 2.0f),
                    CCEaseInOut::create(CCMoveBy::create(0.5f, ccp(0, -10)), 2.0f),
                    nullptr
                ), 2
            ),
            nullptr
        ));
    }
    
    for (int wing = 0; wing < 2; wing++) {
        for (int feather = 0; feather < 6; feather++) {
            auto wingFeather = CCSprite::create("GJ_square01.png");
            if (wingFeather) {
                float side = wing == 0 ? -1.0f : 1.0f;
                float wingAngle = side * (30 + feather * 15);
                float distance = 40 + feather * 10;
                
                CCPoint featherPos = CCPoint(
                    playerPos.x + side * distance * cos(wingAngle * M_PI / 180.0f),
                    playerPos.y + distance * sin(wingAngle * M_PI / 180.0f)
                );
                
                wingFeather->setPosition(featherPos);
                wingFeather->setScale(0.0f);
                wingFeather->setColor(ccc3(255, 255, 255));
                wingFeather->setOpacity(180);
                wingFeather->setZOrder(2700);
                wingFeather->setRotation(wingAngle);
                
                wingFeather->runAction(CCSequence::create(
                    CCDelayTime::create(1.0f + feather * 0.1f),
                    CCSpawn::create(
                        CCScaleTo::create(0.3f, 1.5f + feather * 0.2f),
                        CCFadeIn::create(0.3f),
                        nullptr
                    ),
                    CCDelayTime::create(2.0f),
                    CCSpawn::createWithTwoActions(
                        CCScaleTo::create(1.0f, 0.0f),
                        CCFadeOut::create(1.0f)
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
        lightPillar->setScaleX(2.0f);
        lightPillar->setScaleY(winSize.height / lightPillar->getContentSize().height);
        lightPillar->setColor(ccc3(255, 255, 200));
        lightPillar->setOpacity(0);
        lightPillar->setZOrder(2600);
        
        lightPillar->runAction(CCSequence::create(
            CCDelayTime::create(2.0f),
            CCFadeTo::create(0.5f, 100),
            CCDelayTime::create(1.5f),
            CCFadeOut::create(1.0f),
            CCRemoveSelf::create(),
            nullptr
        ));
        
        playLayer->addChild(lightPillar);
    }
    
    if (mainPlayer) {
        mainPlayer->runAction(CCSequence::create(
            CCDelayTime::create(2.0f),
            CCTintTo::create(0.1f, 255, 255, 255),
            CCSpawn::create(
                CCEaseOut::create(CCMoveBy::create(2.0f, ccp(0, 300)), 2.0f),
                CCSequence::create(
                    CCScaleTo::create(1.0f, 1.2f),
                    CCScaleTo::create(1.0f, 0.8f),
                    nullptr
                ),
                CCSequence::create(
                    CCDelayTime::create(1.0f),
                    CCFadeOut::create(1.0f),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
    
    for (int i = 0; i < 12; i++) {
        auto musicalNote = CCSprite::create("GJ_square01.png");
        if (musicalNote) {
            musicalNote->setPosition(CCPoint(
                playerPos.x + (rand() % 300 - 150),
                playerPos.y + (rand() % 100 - 50)
            ));
            musicalNote->setScale(0.2f + (rand() % 30) / 100.0f);
            musicalNote->setColor(ccc3(255, 255, 200 + rand() % 55));
            musicalNote->setZOrder(2500);
            musicalNote->setOpacity(0);
            
            float floatDirection = (rand() % 2 == 0) ? 1.0f : -1.0f;
            
            musicalNote->runAction(CCSequence::create(
                CCDelayTime::create(3.0f + (rand() % 100) / 100.0f),
                CCFadeIn::create(0.3f),
                CCSpawn::create(
                    CCEaseInOut::create(CCMoveBy::create(2.0f, ccp(floatDirection * 50, 100)), 2.0f),
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
            
            playLayer->addChild(musicalNote);
        }
    }
    
    for (int halo = 0; halo < 3; halo++) {
        auto angelHalo = CCSprite::create("GJ_square01.png");
        if (angelHalo) {
            angelHalo->setPosition(CCPoint(playerPos.x, playerPos.y - 50));
            angelHalo->setScale(0.1f);
            angelHalo->setColor(ccc3(255, 255, 100));
            angelHalo->setOpacity(150);
            angelHalo->setZOrder(2400);
            
            angelHalo->runAction(CCSequence::create(
                CCDelayTime::create(3.5f + halo * 0.3f),
                CCSpawn::create(
                    CCEaseOut::create(CCScaleTo::create(1.5f, 3.0f + halo * 0.5f), 2.0f),
                    CCEaseOut::create(CCMoveBy::create(1.5f, ccp(0, 200 + halo * 50)), 2.0f),
                    CCFadeOut::create(1.5f),
                    nullptr
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(angelHalo);
        }
    }
    
    for (int i = 0; i < 25; i++) {
        auto blessingStar = CCSprite::create("GJ_square01.png");
        if (blessingStar) {
            blessingStar->setPosition(CCPoint(
                playerPos.x + (rand() % 400 - 200),
                playerPos.y + (rand() % 200 - 100)
            ));
            blessingStar->setScale(0.1f);
            blessingStar->setColor(ccc3(255, 255, 255));
            blessingStar->setZOrder(2300);
            blessingStar->setOpacity(0);
            
            blessingStar->runAction(CCSequence::create(
                CCDelayTime::create(4.0f + (rand() % 100) / 100.0f),
                CCSpawn::create(
                    CCFadeIn::create(0.2f),
                    CCScaleTo::create(0.2f, 0.4f),
                    nullptr
                ),
                CCRepeat::create(
                    CCSequence::create(
                        CCScaleTo::create(0.1f, 0.5f),
                        CCScaleTo::create(0.1f, 0.3f),
                        nullptr
                    ), 3
                ),
                CCSpawn::createWithTwoActions(
                    CCFadeOut::create(0.5f),
                    CCScaleTo::create(0.5f, 0.0f)
                ),
                CCRemoveSelf::create(),
                nullptr
            ));
            
            playLayer->addChild(blessingStar);
        }
    }
    
    log::info("👼 ASCENSION COMPLETE: Player has been blessed by the heavens!");
}

void DeathAnimations::createSelectedAnimation(PlayLayer* playLayer, CCPoint playerPos) {
    std::string animationType = Mod::get()->getSettingValue<std::string>("animation-type");
    
    if (animationType == "ascension") {
        createAscensionAnimation(playLayer, playerPos);
    } else {
        createExplosionAnimation(playLayer, playerPos);
    }
}
