#include "ChatOverlay.hpp"
#include "../Network/WebSocketManager.hpp"

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
    m_levelID = levelID;

    float width = 220.0f;
    float height = 110.0f;
    this->setContentSize({width, height});

    m_messages = CCArray::create();
    m_messages->retain();

    auto bg = CCLayerColor::create(ccc4(0, 0, 0, 120), width, height);
    bg->setIgnoreAnchorPointForPosition(false);
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setZOrder(-1);
    this->addChild(bg);

    m_container = CCMenu::create();
    m_container->setPosition({0.0f, 25.0f});
    m_container->setAnchorPoint({0.0f, 0.0f});
    m_container->setContentSize({width, height - 25.0f});
    this->addChild(m_container);

    // Dùng TextInput wrapper chính thức từ Geode UI
    m_inputField = TextInput::create(width - 10.0f, "Press '/' to chat...", "chatFont.fnt");
    m_inputField->setPosition({width / 2.0f, 12.0f});
    m_inputField->setVisible(false);
    this->addChild(m_inputField);

    WebSocketManager::get().setOnMessage([this](std::string const& sender, std::string const& msg, std::string const& hexColor) {
        ccColor3B col = {255, 255, 255};
        if (hexColor == "#FF0000") col = {255, 0, 0};
        else if (hexColor == "#00FF00") col = {0, 255, 0};
        this->addMessage(sender, msg, col);
    });

    return true;
}

void ChatOverlay::addMessage(std::string const& username, std::string const& message, ccColor3B color) {
    auto cell = ChatCell::create(username, message, color);
    if (!cell) return;

    m_container->addChild(cell);
    m_messages->addObject(cell);

    if (m_messages->count() > 6) {
        auto firstCell = static_cast<ChatCell*>(m_messages->objectAtIndex(0));
        m_container->removeChild(firstCell, true);
        m_messages->removeObjectAtIndex(0);
    }

    this->updateLayout();
}

void ChatOverlay::updateLayout() {
    float currentY = 0.0f;
    for (int i = m_messages->count() - 1; i >= 0; --i) {
        auto cell = static_cast<ChatCell*>(m_messages->objectAtIndex(i));
        cell->setPosition({5.0f, currentY});
        currentY += cell->getContentSize().height + 2.0f;
    }
}

void ChatOverlay::toggleInput(bool enable) {
    m_isTyping = enable;
    m_inputField->setVisible(enable);
    if (enable) {
        m_inputField->onClickTrackNode(true);
    } else {
        m_inputField->onClickTrackNode(false);
    }
}

void ChatOverlay::sendCurrentMessage() {
    std::string text = m_inputField->getString();
    if (!text.empty()) {
        WebSocketManager::get().sendMessage(text, m_levelID);
        m_inputField->setString("");
    }
    toggleInput(false);
}