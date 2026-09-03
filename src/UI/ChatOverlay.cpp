#include "ChatOverlay.hpp"
#include "ChatCell.hpp"
#include "SettingsPopup.hpp"

using namespace geode::prelude;

ChatOverlay* ChatOverlay::create(std::string const& roomName, bool readOnly) {
    auto ret = new ChatOverlay();
    if (ret && ret->initAnchored(420.0f, 260.0f, roomName, readOnly)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatOverlay::setup(std::string const& roomName, bool readOnly) {
    m_roomName = roomName;
    m_readOnly = readOnly;

    this->setTitle("DashChat - " + m_roomName);

    auto winSize = m_mainLayer->getContentSize();

    auto topMenu = CCMenu::create();
    topMenu->setPosition({ winSize.width - 25.0f, winSize.height - 25.0f });

    auto settingsBtnSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    if (settingsBtnSprite) {
        settingsBtnSprite->setScale(0.65f);
        auto settingsBtn = CCMenuItemSpriteExtra::create(
            settingsBtnSprite,
            this,
            menu_selector(ChatOverlay::onOpenSettings)
        );
        topMenu->addChild(settingsBtn);
    }
    m_mainLayer->addChild(topMenu);

    m_chatContainer = CCNode::create();
    m_chatContainer->setPosition({ winSize.width / 2.0f, winSize.height / 2.0f + 10.0f });
    m_mainLayer->addChild(m_chatContainer);

    auto demoCell = ChatCell::create(
        "UserTest", 
        "Welcome to " + m_roomName, 
        "https://cdn.discordapp.com/embed/avatars/0.png",
        { 0, 255, 128 }
    );
    if (demoCell) {
        m_chatContainer->addChild(demoCell);
    }

    if (!m_readOnly) {
        auto bottomMenu = CCMenu::create();
        bottomMenu->setPosition({ winSize.width / 2.0f, 25.0f });

        m_messageInput = TextInput::create(280.0f, "Type message...", "chatFont.fnt");
        m_messageInput->setScale(0.75f);
        m_messageInput->setPosition({ -45.0f, 0.0f });

        auto sendBtnSprite = ButtonSprite::create("Send", "goldFont.fnt", "GJ_button_01.png", 0.7f);
        auto sendBtn = CCMenuItemSpriteExtra::create(
            sendBtnSprite,
            this,
            menu_selector(ChatOverlay::onSend)
        );
        sendBtn->setPosition({ 125.0f, 0.0f });

        bottomMenu->addChild(m_messageInput);
        bottomMenu->addChild(sendBtn);
        m_mainLayer->addChild(bottomMenu);
    }

    return true;
}

void ChatOverlay::onSend(CCObject* sender) {
    if (!m_messageInput) return;
    std::string text = m_messageInput->getString();
    if (text.empty()) return;

    auto cell = ChatCell::create("Me", text, "", { 255, 255, 255 });
    if (cell && m_chatContainer) {
        m_chatContainer->addChild(cell);
    }
    m_messageInput->setString("");
}

void ChatOverlay::onOpenSettings(CCObject* sender) {
    SettingsPopup::create()->show();
}