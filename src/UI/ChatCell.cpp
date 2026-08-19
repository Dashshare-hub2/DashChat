#include "ChatCell.hpp"

ChatCell* ChatCell::create(std::string const& username, std::string const& message, ccColor3B userColor) {
    auto ret = new ChatCell();
    if (ret && ret->init(username, message, userColor)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(std::string const& username, std::string const& message, ccColor3B userColor) {
    if (!CCNode::init()) return false;

    float padding = 4.0f;
    std::string userStr = username + ": ";

    m_usernameLabel = CCLabelBMFont::create(userStr.c_str(), "chatFont.fnt");
    m_usernameLabel->setScale(0.40f);
    m_usernameLabel->setColor(userColor);
    m_usernameLabel->setAnchorPoint({0.0f, 0.5f});

    m_messageLabel = CCLabelBMFont::create(message.c_str(), "chatFont.fnt");
    m_messageLabel->setScale(0.40f);
    m_messageLabel->setAnchorPoint({0.0f, 0.5f});

    float userWidth = m_usernameLabel->getScaledContentSize().width;
    m_usernameLabel->setPosition({padding, 10.0f});
    m_messageLabel->setPosition({padding + userWidth, 10.0f});

    this->addChild(m_usernameLabel);
    this->addChild(m_messageLabel);

    float totalHeight = std::max(m_usernameLabel->getScaledContentSize().height, m_messageLabel->getScaledContentSize().height) + padding;
    this->setContentSize({220.0f, totalHeight});

    return true;
}