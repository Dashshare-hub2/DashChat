#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ChatManager.hpp"
#include "ChatPopup.hpp"
#include <sio_client.h>

using namespace geode::prelude;

$on_mod(Loaded) {
    ChatManager::get()->connect("https://dashchat-rsuk.onrender.com");

    ChatManager::get()->setOnInviteCallback([](const std::string& inviter, const std::string& target, const std::string& roomCode) {
        std::string myUsername = GJAccountManager::sharedState()->m_username;
        if (target == myUsername || target == "ALL") {
            Notification::create(
                fmt::format("{} invited you to Chat Room {}", inviter, roomCode),
                NotificationIcon::Info,
                3.0f
            )->show();
        }
    });
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        if (!sprite) {
            sprite = CCSprite::create("GJ_button_01.png");
        }

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::onOpenChat)
        );

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenChat(CCObject*) {
        std::string currentGDUser = GJAccountManager::sharedState()->m_username;
        if (currentGDUser.empty()) currentGDUser = "Guest";

        ChatPopup::create(currentGDUser)->show();
    }
};