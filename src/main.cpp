#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include "DeathAnimations.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        bool m_delayActive = false;
        bool m_showingDelayedBest = false;
        EventListener<Task<bool, int>> m_delayTaskListener;
        bool m_newReward;
        int m_orbs;
        int m_diamonds;
        bool m_demonKey;
        bool m_noRetry;
        bool m_noTitle;
        CCPoint m_deathPosition;
    };
    
    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
        if (m_fields->m_showingDelayedBest) {
            PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
            return;
        }
        
        log::info("NEW BEST ACHIEVED! Starting death animation...");
        
        std::string animationType = Mod::get()->getSettingValue<std::string>("animation-type");
        log::info("Selected animation type: {}", animationType);
        
        DeathAnimations::createSelectedAnimation(this, m_fields->m_deathPosition);
        
        m_fields->m_newReward = newReward;
        m_fields->m_orbs = orbs;
        m_fields->m_diamonds = diamonds;
        m_fields->m_demonKey = demonKey;
        m_fields->m_noRetry = noRetry;
        m_fields->m_noTitle = noTitle;
        
        m_fields->m_delayActive = true;
        
        int delaySeconds = Mod::get()->getSettingValue<int64_t>("delay-duration");
        int delayMs = delaySeconds * 1000;
        
        auto delayTask = Task<bool, int>::run([delayMs](auto progress, auto hasBeenCancelled) -> Task<bool, int>::Result {
            const int checkInterval = 100;
            int elapsed = 0;
            
            while (elapsed < delayMs) {
                if (hasBeenCancelled()) {
                    return Task<bool, int>::Cancel();
                }
                
                progress(static_cast<int>((float)elapsed / delayMs * 100));
                
                std::this_thread::sleep_for(std::chrono::milliseconds(checkInterval));
                elapsed += checkInterval;
            }
            
            return true;
        }, "New Best Delay Task");
        
        m_fields->m_delayTaskListener.bind([this](Task<bool, int>::Event* event) {
            if (event->getValue()) {
                m_fields->m_delayActive = false;
                m_fields->m_showingDelayedBest = true;
                
                this->showNewBest(
                    m_fields->m_newReward,
                    m_fields->m_orbs, 
                    m_fields->m_diamonds,
                    m_fields->m_demonKey,
                    m_fields->m_noRetry,
                    m_fields->m_noTitle
                );
                
                m_fields->m_showingDelayedBest = false;
            }
            return ListenerResult::Propagate;
        });
        
        m_fields->m_delayTaskListener.setFilter(delayTask);
    }
    
    void resetLevel() {
        if (m_fields->m_delayActive) {
            m_fields->m_delayActive = false;
        }
        
        auto player1 = this->m_player1;
        auto player2 = this->m_player2;
        
        if (player1) {
            player1->stopAllActions();
            player1->setScale(1.0f);
            player1->setOpacity(255);
            player1->setColor(ccc3(255, 255, 255));
            player1->setRotation(0.0f);
            player1->setVisible(true);
        }
        
        if (player2) {
            player2->stopAllActions();
            player2->setScale(1.0f);
            player2->setOpacity(255);
            player2->setColor(ccc3(255, 255, 255));
            player2->setRotation(0.0f);
            player2->setVisible(true);
        }
        
        PlayLayer::resetLevel();
    }
    
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        log::info("Player died at position ({}, {})", player->getPosition().x, player->getPosition().y);
        
        m_fields->m_deathPosition = player->getPosition();
        
        PlayLayer::destroyPlayer(player, object);
    }
    
    void delayedResetLevel() {
        if (m_fields->m_delayActive) {
            log::info("Blocked auto-respawn because new best delay is active");
            return;
        }
        
        PlayLayer::delayedResetLevel();
    }
};