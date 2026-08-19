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
    if (!CCNodeRGBA::init()) return false;

    float cellWidth = 210.0f;
    float padding = 4.0f;

  
    m_bg = CCLayerColor::create(ccc4(0, 0, 0, 90), cellWidth, 18.0f);
    m_bg->setIgnoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({0.0f, 0.0f});
    this->addChild(m_bg, -1);

    std::string userStr = username + ": ";
    m_usernameLabel = CCLabelBMFont::create(userStr.c_str(), "chatFont.fnt");
    m_usernameLabel->setScale(0.35f);
    m_usernameLabel->setColor(userColor);
    m_usernameLabel->setAnchorPoint({0.0f, 0.5f});

    m_messageLabel = CCLabelBMFont::create(message.c_str(), "chatFont.fnt");
    m_messageLabel->setScale(0.35f);
    m_messageLabel->setAnchorPoint({0.0f, 0.5f});

    float userWidth = m_usernameLabel->getScaledContentSize().width;
    m_usernameLabel->setPosition({padding, 9.0f});
    m_messageLabel->setPosition({padding + userWidth, 9.0f});

    this->addChild(m_usernameLabel);
    this->addChild(m_messageLabel);

    this->setContentSize({cellWidth, 18.0f});
    return true;
}