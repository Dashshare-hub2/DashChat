#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
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
    if (!CCNode::init()) return false;
    return true;
}

void ChatCell::loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    web::WebRequest req;

    m_avatarListener.bind([parentNode](web::WebResponse* response) {
        if (response && response->ok()) {
            auto data = response->data();
            if (!data.empty() && parentNode) {
                auto image = new CCImage();
                if (image->initWithImageData(const_cast<uint8_t*>(data.data()), data.size())) {
                    auto texture = new CCTexture2D();
                    if (texture->initWithImage(image)) {
                        auto sprite = CCSprite::createWithTexture(texture);
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
    });

    m_avatarListener.setFilter(req.get(avatarUrl));
}