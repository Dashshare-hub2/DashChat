#include "SettingsPopup.hpp"

using namespace geode::prelude;

SettingsPopup* SettingsPopup::create() {
    auto ret = new SettingsPopup();
    if (ret && ret->initAnchored(360.0f, 220.0f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SettingsPopup::setup() {
    this->setTitle("DashChat - Server Settings");

    auto winSize = m_mainLayer->getContentSize();

    // 1. Render Chat Server URL
    auto serverLabel = CCLabelBMFont::create("Render Chat Server:", "bigFont.fnt");
    serverLabel->setScale(0.35f);
    serverLabel->setAnchorPoint({0.0f, 0.5f});
    serverLabel->setPosition({ 30.0f, winSize.height - 50.0f });
    m_mainLayer->addChild(serverLabel);

    std::string currentServer = Mod::get()->getSavedValue<std::string>("chat_server_url", "https://dashchat-rsuk.onrender.com");
    m_serverInput = TextInput::create(300.0f, "Server URL", "chatFont.fnt");
    m_serverInput->setString(currentServer);
    m_serverInput->setPosition({ winSize.width / 2.0f, winSize.height - 75.0f });
    m_serverInput->setScale(0.8f);
    m_mainLayer->addChild(m_serverInput);

    // 2. Firebase Database URL
    auto fbLabel = CCLabelBMFont::create("Firebase RTDB URL:", "bigFont.fnt");
    fbLabel->setScale(0.35f);
    fbLabel->setAnchorPoint({0.0f, 0.5f});
    fbLabel->setPosition({ 30.0f, winSize.height - 105.0f });
    m_mainLayer->addChild(fbLabel);

    std::string currentFb = Mod::get()->getSavedValue<std::string>("firebase_db_url", "https://dashchat-server-default-rtdb.firebaseio.com");
    m_firebaseInput = TextInput::create(300.0f, "Firebase DB URL", "chatFont.fnt");
    m_firebaseInput->setString(currentFb);
    m_firebaseInput->setPosition({ winSize.width / 2.0f, winSize.height - 130.0f });
    m_firebaseInput->setScale(0.8f);
    m_mainLayer->addChild(m_firebaseInput);

    // Action Menu
    auto menu = CCMenu::create();
    menu->setPosition({ winSize.width / 2.0f, 30.0f });

    // Link Discord Button
    auto discordBtnSprite = ButtonSprite::create("Discord", "goldFont.fnt", "GJ_button_02.png", 0.7f);
    auto discordBtn = CCMenuItemSpriteExtra::create(
        discordBtnSprite,
        this,
        menu_selector(SettingsPopup::onLinkDiscord)
    );

    // Save & Connect Button
    auto saveBtnSprite = ButtonSprite::create("Save & Connect", "goldFont.fnt", "GJ_button_01.png", 0.7f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveBtnSprite,
        this,
        menu_selector(SettingsPopup::onSaveAndConnect)
    );

    menu->addChild(discordBtn);
    menu->addChild(saveBtn);
    menu->setLayout(RowLayout::create()->setGap(15.0f));

    m_mainLayer->addChild(menu);

    return true;
}

void SettingsPopup::onSaveAndConnect(CCObject* sender) {
    std::string serverUrl = m_serverInput->getString();
    std::string fbUrl = m_firebaseInput->getString();

    Mod::get()->setSavedValue("chat_server_url", serverUrl);
    Mod::get()->setSavedValue("firebase_db_url", fbUrl);

    FLAlertLayer::create("DashChat", "Settings saved successfully! Reconnecting...", "OK")->show();
    this->onClose(nullptr);
}

void SettingsPopup::onLinkDiscord(CCObject* sender) {
    web::openLinkInBrowser("https://dashchat-rsuk.onrender.com/auth/discord");
}