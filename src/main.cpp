#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/loader/SettingEvent.hpp>

#include "ChatManager.hpp"
#include "ChatLayer.hpp"

using namespace geode::prelude;

static ChatLayer* g_chatOverlay = nullptr;

class $modify(DashChatPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        std::string inviteCode = Mod::get()->getSettingValue<std::string>("invite-code");


        auto chatOverlay = ChatLayer::create(inviteCode);
        if (chatOverlay) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();


            chatOverlay->setPosition({ winSize.width - 220.f, 80.f });
            

            chatOverlay->setVisible(false);

            this->addChild(chatOverlay, 9999); 
            g_chatOverlay = chatOverlay;
        }

        return true;
    }

    void onExit() {
        g_chatOverlay = nullptr;
        PlayLayer::onExit();
    }
};


class $modify(DashChatPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        
        auto spr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        if (!spr) spr = ButtonSprite::create("Chat", "goldFont.fnt", "GJ_button_01.png", 0.7f);

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(DashChatPauseLayer::onToggleChatInGame)
        );

        auto dashChatMenu = CCMenu::create();
        dashChatMenu->setID("dashchat-pause-button");
        dashChatMenu->addChild(btn);


        auto winSize = CCDirector::sharedDirector()->getWinSize();
        dashChatMenu->setPosition({ (winSize.width / 2.0f) - 185.0f, (winSize.height / 2.0f) - 105.0f });

        this->addChild(dashChatMenu, 999);
    }

    void onToggleChatInGame(CCObject*) {
        if (g_chatOverlay) {
            bool isVisible = g_chatOverlay->isVisible();
            g_chatOverlay->setVisible(!isVisible);
        }

        this->onResume(nullptr);
    }
};

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