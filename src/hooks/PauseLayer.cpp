#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("left-button-menu");
        if (!menu) {

            menu = CCMenu::create();
            menu->setID("dashchat-left-menu"_spr);
            menu->setPosition({30.0f, 30.0f});
            this->addChild(menu);
        }

        auto chatSprite = CircleButtonSprite::createWithSpriteFrameName(
            "geode.loader/geode-logo-outline.png",
            1.0f,
            CircleBaseColor::Green,
            CircleBaseSize::Medium
        );

        auto chatBtn = CCMenuItemSpriteExtra::create(
            chatSprite,
            this,
            menu_selector(MyPauseLayer::onDashChat)
        );
        chatBtn->setID("dashchat-pause-btn"_spr);

        menu->addChild(chatBtn);
        menu->updateLayout();
    }

    void onDashChat(CCObject* sender) {
        web::openURL("https://dashchat-rsuk.onrender.com");
    }
};