#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../UI/ChatOverlay.hpp"
#include "../Network/WebSocketManager.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        ChatOverlay* m_chatOverlay = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        WebSocketManager::get().connect();

        CCNode* parentNode = this->m_uiLayer ? static_cast<CCNode*>(this->m_uiLayer) : static_cast<CCNode*>(this);

        std::string levelID = std::to_string(level->m_levelID.value());
        auto overlay = ChatOverlay::create(levelID);
        overlay->setID("dashchat-overlay"_spr);
        

        overlay->setPosition({10.0f, 40.0f});
        overlay->setZOrder(99999); 

        parentNode->addChild(overlay);
        m_fields->m_chatOverlay = overlay;

        return true;
    }

    void keyDown(enumKeyCodes key) {
        if (m_fields->m_chatOverlay) {
            if (key == KEY_Slash && !m_fields->m_chatOverlay->isTyping()) {
                m_fields->m_chatOverlay->toggleInput(true);
                return;
            }
            if ((key == KEY_Enter || key == KEY_KeypadEnter) && m_fields->m_chatOverlay->isTyping()) {
                m_fields->m_chatOverlay->sendCurrentMessage();
                return;
            }
        }
        PlayLayer::keyDown(key);
    }
};