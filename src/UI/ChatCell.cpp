#include "ChatCell.hpp"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

ChatCell* ChatCell::create(std::string const& sender, std::string const& text, cocos2d::ccColor3B color) {
    auto ret = new ChatCell();
    if (ret && ret->init(sender, text, color)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(std::string const& sender, std::string const& text, cocos2d::ccColor3B color) {
    if (!CCNode::init()) return false;

    this->setContentSize({ 320.0f, 30.0f });

    m_senderLabel = CCLabelBMFont::create((sender + ": ").c_str(), "goldFont.fnt");
    m_senderLabel->setScale(0.45f);
    m_senderLabel->setAnchorPoint({ 0.0f, 0.5f });
    m_senderLabel->setPosition({ 5.0f, 15.0f });
    this->addChild(m_senderLabel);

    m_messageLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    m_messageLabel->setScale(0.45f);
    m_messageLabel->setColor(color);
    m_messageLabel->setAnchorPoint({ 0.0f, 0.5f });
    m_messageLabel->setPosition({ m_senderLabel->getPositionX() + m_senderLabel->getScaledContentSize().width, 15.0f });
    this->addChild(m_messageLabel);

    return true;
}

void ChatCell::loadDiscordAvatar(ChatCell* cell, std::string const& avatarUrl) {
    if (avatarUrl.empty()) return;

    web::WebRequestReq()
        .get(avatarUrl)
        .listen([this](web::WebResponseRes response) {
            if (response && response->ok()) {
                auto data = response->data();
                auto img = new CCImage();
                if (img->initWithImageData(const_cast<uint8_t*>(data.data()), data.size())) {
                    auto texture = new CCTexture2D();
                    if (texture->initWithImage(img)) {
                        if (m_avatarSprite) m_avatarSprite->removeFromParent();
                        m_avatarSprite = CCSprite::createWithTexture(texture);
                        m_avatarSprite->setScale(20.0f / m_avatarSprite->getContentSize().width);
                        m_avatarSprite->setPosition({ -10.0f, 15.0f });
                        this->addChild(m_avatarSprite);
                    }
                    texture->release();
                }
                img->release();
            }
        });
}