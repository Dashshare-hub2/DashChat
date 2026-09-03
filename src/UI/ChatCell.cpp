#include "ChatCell.hpp"

using namespace geode::prelude;

ChatCell* ChatCell::create(
    std::string const& sender, 
    std::string const& text, 
    std::string const& avatarUrl, 
    cocos2d::ccColor3B color
) {
    auto ret = new ChatCell();
    if (ret && ret->init(sender, text, avatarUrl, color)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(
    std::string const& sender, 
    std::string const& text, 
    std::string const& avatarUrl, 
    cocos2d::ccColor3B color
) {
    if (!CCNode::init()) return false;

    this->setContentSize({350.0f, 35.0f});

    // Avatar Icon
    m_avatarSprite = CCSprite::createWithSpriteFrameName("gj_commentIcon_001.png");
    if (m_avatarSprite) {
        m_avatarSprite->setPosition({ 18.0f, 17.0f });
        m_avatarSprite->setScale(0.7f);
        this->addChild(m_avatarSprite);
    }

    // Username Label
    m_usernameLabel = CCLabelBMFont::create(sender.c_str(), "goldFont.fnt");
    if (m_usernameLabel) {
        m_usernameLabel->setAnchorPoint({ 0.0f, 0.5f });
        m_usernameLabel->setPosition({ 40.0f, 24.0f });
        m_usernameLabel->setScale(0.45f);
        m_usernameLabel->setColor(color);
        this->addChild(m_usernameLabel);
    }

    // Message Text Label
    m_messageLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    if (m_messageLabel) {
        m_messageLabel->setAnchorPoint({ 0.0f, 0.5f });
        m_messageLabel->setPosition({ 40.0f, 10.0f });
        m_messageLabel->setScale(0.38f);
        this->addChild(m_messageLabel);
    }

    if (!avatarUrl.empty()) {
        this->loadDiscordAvatar(avatarUrl);
    }

    return true;
}

void ChatCell::loadDiscordAvatar(std::string const& avatarUrl) {
    m_avatarTaskListener.bind([this](web::WebTask::Event* event) {
        if (auto res = event->getValue()) {
            if (!res->ok()) return;
            auto data = res->data();
            if (data.empty()) return;

            auto img = new cocos2d::CCImage();
            if (img->initWithImageData(const_cast<uint8_t*>(data.data()), data.size())) {
                auto texture = new cocos2d::CCTexture2D();
                if (texture->initWithImage(img)) {
                    if (m_avatarSprite) {
                        m_avatarSprite->setTexture(texture);
                        auto size = texture->getContentSize();
                        if (size.width > 0 && size.height > 0) {
                            m_avatarSprite->setTextureRect(cocos2d::CCRect(0, 0, size.width, size.height));
                            m_avatarSprite->setScale(24.0f / size.width);
                        }
                    }
                    texture->release();
                }
                img->release();
            }
        }
    });

    auto req = web::WebRequest();
    m_avatarTaskListener.setFilter(req.get(avatarUrl));
}