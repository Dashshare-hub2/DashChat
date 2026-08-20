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

    m_messages = CCArray::create();
    m_messages->retain();

    float width = 180.0f;
    float height = 90.0f;
    this->setContentSize({width, height});

    auto bg = CCLayerColor::create(ccc4(0, 0, 0, 120), width, height);
    bg->ignoreAnchorPointForPosition(false);
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setZOrder(-1);
    this->addChild(bg);

    m_container = CCMenu::create();
    m_container->setPosition({0.0f, 22.0f});
    m_container->setAnchorPoint({0.0f, 0.0f});
    m_container->setContentSize({width, height - 22.0f});
    this->addChild(m_container);

    m_inputField = TextInput::create(width - 10.0f, "Press '/' to chat...", "chatFont.fnt");
    m_inputField->setPosition({width / 2.0f, 11.0f});
    m_inputField->setScale(0.7f);
    m_inputField->setVisible(false);

    m_inputField->setCallback([this](std::string const& text) {
    });

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

    if (m_messages->count() > 4) {
        auto firstCell = static_cast<ChatCell*>(m_messages->objectAtIndex(0));
        m_container->removeChild(firstCell, true);
        m_messages->removeObjectAtIndex(0);
    }

    this->updateLayout();
}

void ChatOverlay::updateLayout() {
    float y = 0.0f;
    for (int i = m_messages->count() - 1; i >= 0; --i) {
        auto cell = static_cast<ChatCell*>(m_messages->objectAtIndex(i));
        cell->setPosition({5.0f, y});
        y += 15.0f;
    }
}

void ChatOverlay::toggleTyping(bool typing) {
    m_isTyping = typing;
    m_inputField->setVisible(typing);

    if (typing) {
        m_inputField->setEnabled(true);
        m_inputField->onClickTrackNode(true); 
    } else {
        m_inputField->onClickTrackNode(false);
        m_inputField->setEnabled(false);
        this->sendMessage();
    }
}

void ChatOverlay::sendMessage() {
    std::string text = m_inputField->getString();
    if (!text.empty()) {
        m_inputField->setString("");
    }
}

bool ChatOverlay::isTyping() const {
    return m_isTyping;
}

ChatOverlay::~ChatOverlay() {
    CC_SAFE_RELEASE(m_messages);
}
