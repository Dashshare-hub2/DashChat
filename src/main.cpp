#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/loader/SettingEvent.hpp>

#include "ChatManager.hpp"
#include "ChatLayer.hpp"

using namespace geode::prelude;

class $modify(DashChatMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        ChatManager::get()->connect();

        auto spr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        if (!spr) spr = ButtonSprite::create("Chat");

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(DashChatMenuLayer::onOpenDashChat)
        );

        auto rightMenu = this->getChildByID("right-side-menu");
        if (rightMenu) {
            rightMenu->addChild(btn);
            rightMenu->updateLayout();
        }

        return true;
    }

    void onOpenDashChat(CCObject*) {
        std::string inviteCode = Mod::get()->getSettingValue<std::string>("invite-code");
        ChatLayer::create(inviteCode)->show();
    }
};


class $modify(DashChatPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto spr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        if (!spr) spr = ButtonSprite::create("Chat", "goldFont.fnt", "GJ_button_01.png", 0.8f);

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(DashChatPauseLayer::onOpenDashChat)
        );

        auto chatMenu = CCMenu::create();
        chatMenu->setID("dashchat-pause-menu");
        chatMenu->addChild(btn);

        auto winSize = CCDirector::sharedDirector()->getWinSize();


        float xPos = (winSize.width / 2.0f) - 185.0f;
        float yPos = (winSize.height / 2.0f) - 105.0f;

        chatMenu->setPosition({ xPos, yPos });

        this->addChild(chatMenu, 999);
    }

    void onOpenDashChat(CCObject*) {
        std::string inviteCode = Mod::get()->getSettingValue<std::string>("invite-code");
        ChatLayer::create(inviteCode)->show();
    }
};

$execute {
    listenForSettingChanges("enable-chat", [](bool enabled) {
        if (enabled) ChatManager::get()->connect();
        else ChatManager::get()->disconnect();
    });

    listenForSettingChanges("server-url", [](std::string const&) {
        ChatManager::get()->connect();
    });

    listenForSettingChanges("invite-code", [](std::string const&) {
        ChatManager::get()->connect();
    });
}