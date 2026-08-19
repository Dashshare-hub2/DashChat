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
        if (!spr) spr = ButtonSprite::create("Chat");

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(DashChatPauseLayer::onOpenDashChat)
        );

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) {
            rightMenu = this->getChildByID("bottom-right-menu");
        }

        if (rightMenu) {
            rightMenu->addChild(btn);
            rightMenu->updateLayout();
        } else {
            auto menu = CCMenu::create();
            menu->setPosition({CCDirector::sharedDirector()->getWinSize().width - 40.f, 100.f});
            menu->addChild(btn);
            this->addChild(menu);
        }
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