#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../UI/ChatOverlay.hpp"

using namespace geode::prelude;

class $modify(DashPlayLayer, PlayLayer) {
    struct Fields {
        ChatOverlay* m_chatOverlay = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        m_fields->m_chatOverlay = ChatOverlay::create("Global", true);
        if (m_fields->m_chatOverlay) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            m_fields->m_chatOverlay->setPosition({ winSize.width - 370.0f, winSize.height - 190.0f });
            this->addChild(m_fields->m_chatOverlay, 9999);
        }

        return true;
    }
};