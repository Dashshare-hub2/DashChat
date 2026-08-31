#include "ChatOverlay.hpp"
#include "ChatCell.hpp"

using namespace geode::prelude;

ChatOverlay* ChatOverlay::create(std::string const& roomName, bool readOnly) {
    auto ret = new ChatOverlay();
    if (ret && ret->initAnchored(400.0f, 260.0f, roomName, readOnly)) {
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

    auto chatContainer = CCNode::create();
    chatContainer->setPosition({winSize.width / 2.0f, winSize.height / 2.0f + 10.0f});
    m_mainLayer->addChild(chatContainer);

    auto demoCell = ChatCell::create(
        "UserTest", 
        "Xin chào từ Geode v5!", 
        "https://cdn.discordapp.com/embed/avatars/0.png"
    );
    if (demoCell) {
        chatContainer->addChild(demoCell);
    }

    if (!m_readOnly) {
        auto menu = CCMenu::create();
        menu->setPosition({winSize.width / 2.0f, 30.0f});

        auto sendBtnSprite = ButtonSprite::create("Send", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto sendBtn = CCMenuItemSpriteExtra::create(
            sendBtnSprite,
            this,
            menu_selector(ChatOverlay::onSend)
        );

        menu->addChild(sendBtn);
        m_mainLayer->addChild(menu);
    }

    return true;
}

void ChatOverlay::onSend(CCObject* sender) {
    FLAlertLayer::create("DashChat", "Sent!!!", "OK")->show();
}