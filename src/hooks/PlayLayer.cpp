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

        m_fields->m_chatOverlay = ChatOverlay::create(std::to_string(level->m_levelID.value()));
        m_fields->m_chatOverlay->setPosition({10.0f, 10.0f});
        this->addChild(m_fields->m_chatOverlay, 100);

        return true;
    }

    void keyDown(enumKeyCodes key, bool isRepeat) {
        if (key == enumKeyCodes::KEY_Slash && !m_fields->m_chatOverlay->isTyping()) {
            m_fields->m_chatOverlay->toggleTyping(true);
            return;
        }

        if ((key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::KEY_NumPadEnter) && m_fields->m_chatOverlay->isTyping()) {
            m_fields->m_chatOverlay->toggleTyping(false);
            return;
        }

        PlayLayer::keyDown(key, isRepeat);
    }
};