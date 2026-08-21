#include "ChatOverlay.hpp"
#include "ChatCell.hpp"
#include "../Network/WebSocketManager.hpp"

using namespace geode::prelude;

ChatOverlay* ChatOverlay::create(std::string const& roomName) {
    auto ret = new ChatOverlay();
    if (ret && ret->initAnchored(360.0f, 220.0f, roomName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatOverlay::setup(std::string const& roomName) {
    this->setTitle("DashChat - " + roomName);

    m_scrollView = cocos2d::extension::CCScrollView::create({ 330.0f, 140.0f });
    m_scrollView->setPosition({ 15.0f, 50.0f });
    m_scrollView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
    
    m_chatContainer = CCNode::create();
    m_scrollView->setContainer(m_chatContainer);
    this->m_mainLayer->addChild(m_scrollView);

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setContentSize({ 330.0f, 140.0f });
    bg->setPosition({ 180.0f, 120.0f });
    bg->setOpacity(100);
    this->m_mainLayer->addChild(bg, -1);

    m_inputNode = TextInput::create(250.0f, "Message...", "chatFont.fnt");
    m_inputNode->setPosition({ 140.0f, 25.0f });
    this->m_mainLayer->addChild(m_inputNode);

    auto sendBtnSprite = ButtonSprite::create("Send", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto sendBtn = CCMenuItemSpriteExtra::create(sendBtnSprite, this, menu_selector(ChatOverlay::onSend));
    
    auto menu = CCMenu::create();
    menu->setPosition({ 300.0f, 25.0f });
    menu->addChild(sendBtn);
    this->m_mainLayer->addChild(menu);

    WebSocketManager::get().connect();
    WebSocketManager::get().setOnMessage([this](std::string const& sender, std::string const& text, std::string const& avatarUrl) {
        this->addChatMessage(sender, text, avatarUrl);
    });

    return true;
}

void ChatOverlay::onSend(cocos2d::CCObject* sender) {
    if (!m_inputNode) return;
    
    std::string text = m_inputNode->getString();
    if (!text.empty()) {
        WebSocketManager::get().send(text);
        m_inputNode->setString("");
    }
}

void ChatOverlay::addChatMessage(std::string const& sender, std::string const& text, std::string const& avatarUrl) {
    auto cell = ChatCell::create(sender, text, { 255, 255, 255 });
    if (!cell) return;

    cell->setPosition({ 0.0f, m_chatHeight });
    m_chatContainer->addChild(cell);

    if (!avatarUrl.empty()) {
        cell->loadDiscordAvatar(cell, avatarUrl);
    }

    m_chatHeight += 35.0f;

    if (m_chatHeight > 140.0f) {
        m_chatContainer->setContentSize({ 330.0f, m_chatHeight });
    } else {
        m_chatContainer->setContentSize({ 330.0f, 140.0f });
    }
}