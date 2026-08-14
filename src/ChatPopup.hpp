#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include "ChatManager.hpp"
#include "ChatCell.hpp"

using namespace geode::prelude;

class ChatPopup : public Popup<std::string const&> {
protected:
    ScrollLayer* m_scrollLayer = nullptr;
    TextInput* m_messageInput = nullptr;
    TextInput* m_roomInput = nullptr;
    CCLabelBMFont* m_roomLabel = nullptr;

    bool setup(std::string const& gdUsername) override {
        this->setTitle("DashChat Global & Private");

        auto winSize = CCDirector::sharedDirector()->getWinSize();


        m_scrollLayer = ScrollLayer::create({340.f, 160.f});
        m_scrollLayer->setPosition({winSize.width / 2 - 170.f, winSize.height / 2 - 60.f});
        this->m_mainLayer->addChild(m_scrollLayer);

        m_messageInput = TextInput::create(240.f, "Type message here...");
        m_messageInput->setPosition({winSize.width / 2 - 30.f, winSize.height / 2 - 85.f});
        this->m_mainLayer->addChild(m_messageInput);

        auto sendBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Send", "goldFont.fnt", "GJ_button_01.png", 0.8f),
            this,
            menu_selector(ChatPopup::onSendMessage)
        );
        auto menu = CCMenu::create();
        menu->addChild(sendBtn);
        menu->setPosition({winSize.width / 2 + 120.f, winSize.height / 2 - 85.f});
        this->m_mainLayer->addChild(menu);

        m_roomInput = TextInput::create(100.f, "6-Digit Code");
        m_roomInput->setPosition({winSize.width / 2 - 100.f, winSize.height / 2 + 95.f});
        this->m_mainLayer->addChild(m_roomInput);

        auto joinRoomBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Join Room", "shortFont.fnt", "GJ_button_02.png", 0.7f),
            this,
            menu_selector(ChatPopup::onJoinPrivateRoom)
        );

        auto joinGlobalBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Global", "shortFont.fnt", "GJ_button_04.png", 0.7f),
            this,
            menu_selector(ChatPopup::onJoinGlobal)
        );

        auto discordAuthBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Link Discord", "shortFont.fnt", "GJ_button_05.png", 0.7f),
            this,
            menu_selector(ChatPopup::onDiscordAuth)
        );

        auto topMenu = CCMenu::create();
        topMenu->addChild(joinRoomBtn);
        topMenu->addChild(joinGlobalBtn);
        topMenu->addChild(discordAuthBtn);
        topMenu->alignItemsHorizontallyWithPadding(10.f);
        topMenu->setPosition({winSize.width / 2 + 50.f, winSize.height / 2 + 95.f});
        this->m_mainLayer->addChild(topMenu);

        ChatManager::get()->setOnMessageCallback([this](const ChatMessage& msg) {
            auto cell = ChatCell::create(msg, 340.f);
            m_scrollLayer->m_contentLayer->addChild(cell);
            m_scrollLayer->m_contentLayer->setLayout(
                ColumnLayout::create()
                    ->setAxisReverse(true)
                    ->setAxisAlignment(AxisAlignment::End)
                    ->setGap(5.f)
            );
            m_scrollLayer->m_contentLayer->updateLayout();
        });

        return true;
    }

    void onSendMessage(CCObject*) {
        std::string text = m_messageInput->getString();
        if (!text.empty()) {
            ChatManager::get()->sendMessage(text);
            m_messageInput->setString("");
        }
    }

    void onJoinPrivateRoom(CCObject*) {
        std::string code = m_roomInput->getString();
        if (code.length() == 6) {
            ChatManager::get()->joinRoom(code);
            FLAlertLayer::create("Room Joined", fmt::format("Joined private room: {}", code), "OK")->show();
        } else {
            FLAlertLayer::create("Error", "Please enter a valid 6-digit room code!", "OK")->show();
        }
    }

    void onJoinGlobal(CCObject*) {
        ChatManager::get()->joinGlobal();
        FLAlertLayer::create("Room Switched", "Switched back to Global Chat.", "OK")->show();
    }

    void onDiscordAuth(CCObject*) {
        std::string url = "https://dashchat-rsuk.onrender.com/auth/discord?gdUser=" + GJAccountManager::sharedState()->m_username;
        web::openLinkInBrowser(url);
    }

public:
    static ChatPopup* create(std::string const& gdUsername) {
        auto ret = new ChatPopup();
        if (ret && ret->initAnchored(380.f, 240.f, gdUsername)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};