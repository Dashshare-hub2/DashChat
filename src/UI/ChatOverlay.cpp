#include "ChatOverlay.hpp"
#include "../Network/WebSocketManager.hpp"

using namespace geode::prelude;

void ChatOverlay::sendMessage() {
    std::string text = m_inputField->getString();
    
    if (!text.empty()) {
        WebSocketManager::get().send(text);

        m_inputField->setString("");
    }
}

void ChatOverlay::toggleTyping(bool typing) {
    m_isTyping = typing;
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