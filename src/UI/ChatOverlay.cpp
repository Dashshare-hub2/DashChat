#include "ChatOverlay.hpp"
#include "ChatCell.hpp"
#include "../Network/WebSocketManager.hpp"

using namespace geode::prelude;

ChatOverlay* ChatOverlay::create(std::string const& roomName, bool readOnly) {
    auto ret = new ChatOverlay();
    float width = 360.0f;
    float height = readOnly ? 180.0f : 230.0f;

    if (ret && ret->initAnchored(width, height, roomName, readOnly)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatOverlay::setup(std::string const& roomName, bool readOnly) {
    m_isReadOnly = readOnly;
    this->setTitle("DashChat - " + roomName);

    float scrollHeight = readOnly ? 130.0f : 140.0f;
    float scrollPosY = readOnly ? 15.0f : 50.0f;

    m_scrollView = cocos2d::extension::CCScrollView::create({ 330.0f, scrollHeight });
    m_scrollView->setPosition({ 15.0f, scrollPosY });
    m_scrollView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
    
    m_chatContainer = CCNode::create();
    m_scrollView->setContainer(m_chatContainer);
    this->m_mainLayer->addChild(m_scrollView);

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setContentSize({ 330.0f, scrollHeight });
    bg->setPosition({ 180.0f, scrollPosY + (scrollHeight / 2.0f) });
    bg->setOpacity(80);
    this->m_mainLayer->addChild(bg, -1);

    if (!m_isReadOnly) {
        m_inputNode = TextInput::create(240.0f, "Type a message...", "chatFont.fnt");
        m_inputNode->setPosition({ 135.0f, 22.0f });
        this->m_mainLayer->addChild(m_inputNode);

        auto sendBtnSprite = ButtonSprite::create("Send", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto sendBtn = CCMenuItemSpriteExtra::create(sendBtnSprite, this, menu_selector(ChatOverlay::onSend));
        
        auto menu = CCMenu::create();
        menu->setPosition({ 295.0f, 22.0f });
        menu->addChild(sendBtn);
        this->m_mainLayer->addChild(menu);
    }

    WebSocketManager::get().connect();
    WebSocketManager::get().setOnMessage([this](std::string const& sender, std::string const& text, std::string const& avatarUrl) {
        this->addChatMessage(sender, text, avatarUrl);
    });

    return true;
}

void ChatOverlay::onSend(cocos2d::CCObject* sender) {
    if (m_isReadOnly || !m_inputNode) return;
    
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

    m_chatHeight += 32.0f;
    float minHeight = m_isReadOnly ? 130.0f : 140.0f;

    if (m_chatHeight > minHeight) {
        m_chatContainer->setContentSize({ 330.0f, m_chatHeight });
    } else {
        m_chatContainer->setContentSize({ 330.0f, minHeight });
    }
}