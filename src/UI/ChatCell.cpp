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

    this->setContentSize({350.0f, 40.0f});

    m_avatarSprite = CCSprite::createWithSpriteFrameName("gj_commentIcon_001.png");
    if (m_avatarSprite) {
        m_avatarSprite->setPosition({20.0f, 20.0f});
        m_avatarSprite->setScale(0.8f);
        this->addChild(m_avatarSprite);
    }

    m_usernameLabel = CCLabelBMFont::create(sender.c_str(), "goldFont.fnt");
    if (m_usernameLabel) {
        m_usernameLabel->setAnchorPoint({0.0f, 0.5f});
        m_usernameLabel->setPosition({45.0f, 28.0f});
        m_usernameLabel->setScale(0.5f);
        m_usernameLabel->setColor(color);
        this->addChild(m_usernameLabel);
    }

    m_messageLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    if (m_messageLabel) {
        m_messageLabel->setAnchorPoint({0.0f, 0.5f});
        m_messageLabel->setPosition({45.0f, 12.0f});
        m_messageLabel->setScale(0.4f);
        this->addChild(m_messageLabel);
    }

    if (!avatarUrl.empty()) {
        this->loadDiscordAvatar(avatarUrl);
    }

    return true;
}

void ChatCell::loadDiscordAvatar(std::string const& avatarUrl) {
    m_avatarTask.bind([this](geode::utils::web::WebResponse* res) {
        if (!res || !res->ok()) return;

        auto data = res->data();
        if (data.empty()) return;

        auto img = new CCImage();
        if (img->initWithImageData(const_cast<uint8_t*>(data.data()), data.size())) {
            auto texture = new CCTexture2D();
            if (texture->initWithImage(img)) {
                if (m_avatarSprite) {
                    m_avatarSprite->setTexture(texture);
                    
                    auto size = texture->getContentSize();
                    if (size.width > 0 && size.height > 0) {
                        m_avatarSprite->setTextureRect(cocos2d::CCRect(0, 0, size.width, size.height));
                        m_avatarSprite->setScale(30.0f / size.width);
                    }
                }
                texture->release();
            }
            img->release();
        }
    });

    auto req = geode::utils::web::WebRequest();
    m_avatarTask.setFilter(req.get(avatarUrl));
}