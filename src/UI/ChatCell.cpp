#include "ChatCell.hpp"

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
    if (!cocos2d::CCNode::init()) return false;

    this->setContentSize({ 300.0f, 30.0f });

    auto senderLabel = cocos2d::CCLabelBMFont::create((sender + ": ").c_str(), "chatFont.fnt");
    if (senderLabel) {
        senderLabel->setAnchorPoint({ 0.0f, 0.5f });
        senderLabel->setPosition({ 35.0f, 15.0f });
        senderLabel->setColor(color);
        senderLabel->setScale(0.5f);
        this->addChild(senderLabel);

        auto textLabel = cocos2d::CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        if (textLabel) {
            textLabel->setAnchorPoint({ 0.0f, 0.5f });
            textLabel->setPosition({ senderLabel->getPositionX() + senderLabel->getScaledContentSize().width, 15.0f });
            textLabel->setScale(0.5f);
            this->addChild(textLabel);
        }
    }

    return true;
}

void ChatCell::loadDiscordAvatar(cocos2d::CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    auto req = web::WebRequest();
    
    m_avatarListener.spawn(
        req.get(avatarUrl),
        [parentNode](web::WebResponse response) {
            if (response.ok()) {
                auto data = response.data();
                if (!data.empty() && parentNode) {
                    auto image = new cocos2d::CCImage();
                    if (image->initWithImageData(const_cast<uint8_t*>(data.data()), data.size())) {
                        auto texture = new cocos2d::CCTexture2D();
                        if (texture->initWithImage(image)) {
                            auto sprite = cocos2d::CCSprite::createWithTexture(texture);
                            if (sprite) {
                                float scale = 20.0f / sprite->getContentSize().width;
                                sprite->setScale(scale);
                                sprite->setPosition({ 15.0f, 15.0f });
                                parentNode->addChild(sprite);
                            }
                            texture->release();
                        }
                        image->release();
                    }
                }
            }
        }
    );
}