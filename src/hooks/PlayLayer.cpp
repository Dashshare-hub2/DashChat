#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../UI/ChatOverlay.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer), public cocos2d::CCKeyboardDelegate {
    struct Fields {
        ChatOverlay* m_chatOverlay = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (level) {
            CCKeyboardDispatcher::sharedDispatcher()->addDelegate(this);

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto overlay = ChatOverlay::create(std::to_string(level->m_levelID.value()));
            
            if (overlay) {
                m_fields->m_chatOverlay = overlay;
                m_fields->m_chatOverlay->setPosition({ winSize.width - 190.0f, winSize.height - 100.0f });
                this->addChild(m_fields->m_chatOverlay, 9999);
            }
        }

        return true;
    }

    void onExit() {
        CCKeyboardDispatcher::sharedDispatcher()->removeDelegate(this);
        PlayLayer::onExit();
    }

    void keyDown(enumKeyCodes key) override {
        if (m_fields->m_chatOverlay) {
            if (key == enumKeyCodes::KEY_Tab) {
                bool isTyping = m_fields->m_chatOverlay->isTyping();
                m_fields->m_chatOverlay->toggleTyping(!isTyping);
                return;
            }

            if ((key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::KEY_NumEnter) && m_fields->m_chatOverlay->isTyping()) {
                m_fields->m_chatOverlay->toggleTyping(false);
                return;
            }
        }
    }
};