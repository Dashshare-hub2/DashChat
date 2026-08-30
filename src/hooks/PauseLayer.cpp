#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../UI/ChatOverlay.hpp"

using namespace geode::prelude;

class $modify(DashPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-side-menu");
        if (!menu) menu = this->getChildByID("bottom-menu");

        if (menu) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName("chatFont.fnt", 0.8f);
            if (!btnSprite) {
                btnSprite = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png"));
            }

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(DashPauseLayer::onOpenChat)
            );
            btn->setID("dashchat-button"_spr);
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    void onOpenChat(CCObject* sender) {
        auto chatPopup = ChatOverlay::create("Global", false);
        if (chatPopup) {
            chatPopup->show();
        }
    }
};