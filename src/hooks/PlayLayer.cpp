#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../UI/ChatOverlay.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        ChatOverlay* m_chatOverlay = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_chatOverlay = ChatOverlay::create(std::to_string(level->m_levelID.value()));
        
        float xPos = winSize.width - 190.0f;
        float yPos = winSize.height - 100.0f;
        
        m_fields->m_chatOverlay->setPosition({xPos, yPos});
        this->addChild(m_fields->m_chatOverlay, 100);

        return true;
    }

    void keyDown(enumKeyCodes key, bool isRepeat) {
        if (key == enumKeyCodes::KEY_Slash && !m_fields->m_chatOverlay->isTyping()) {
            m_fields->m_chatOverlay->toggleTyping(true);
            return;
        }

        if ((key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::KEY_NumEnter) && m_fields->m_chatOverlay->isTyping()) {
            m_fields->m_chatOverlay->toggleTyping(false);
            return;
        }

        PlayLayer::keyDown(key, isRepeat);
    }
};