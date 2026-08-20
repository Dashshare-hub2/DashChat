#include "ChatOverlay.hpp"
#include "../Network/WebSocketManager.hpp"

using namespace geode::prelude;

ChatOverlay* ChatOverlay::create(std::string const& levelID) {
    auto ret = new ChatOverlay();
    if (ret && ret->init(levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatOverlay::init(std::string const& levelID) {
    if (!CCNode::init()) return false;

 
    this->setContentSize({ 180.0f, 90.0f });


    auto bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    bg->setContentSize(this->getContentSize());
    bg->setPosition(this->getContentSize() / 2);
    bg->setOpacity(120); 
    bg->setColor({ 0, 0, 0 });
    this->addChild(bg);


    m_inputField = TextInput::create(170.0f, "Press Tab to chat...", "chatFont.fnt");
    m_inputField->setPosition({ 90.0f, 15.0f });
    m_inputField->setMaxCharCount(100);
    m_inputField->setEnabled(false); 
    this->addChild(m_inputField);

    WebSocketManager::get().setOnMessage([this](std::string const& sender, std::string const& text, std::string const& color) {
        geode::log::info("[Chat] {}: {}", sender, text);
    });

    return true;
}

void ChatOverlay::sendMessage() {
    if (!m_inputField) return;

    std::string text = m_inputField->getString();
    if (!text.empty()) {
        WebSocketManager::get().send(text);
        m_inputField->setString(""); 
    }
}

void ChatOverlay::toggleTyping(bool typing) {
    m_isTyping = typing;
    if (!m_inputField) return;

    m_inputField->setEnabled(typing);

    if (auto inputNode = m_inputField->getInputNode()) {
        if (typing) {
            inputNode->attachWithIME(); 
        } else {
            inputNode->detachWithIME(); 
            this->sendMessage();        
        }
    }
}