#include "ChatCell.hpp"

using namespace geode::prelude;

ChatCell* ChatCell::create() {
    auto ret = new ChatCell();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init() {
    if (!CCNode::init()) return false;
    return true;
}

void ChatCell::loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    m_avatarListener.bind([parentNode](web::WebResponseEvent* event) {
        if (auto response = event->getValue()) {
            if (response->ok()) {
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
        }
    });

    web::WebRequest req;
    m_avatarListener.setFilter(req.get(avatarUrl));
}