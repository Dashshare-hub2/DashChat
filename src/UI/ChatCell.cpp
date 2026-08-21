#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

void ChatCell::loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    auto req = web::WebRequest();
    
    m_listener.spawn(
        "Loading Avatar",
        req.get(avatarUrl),
        [parentNode](web::WebResponse response) {
            if (response.ok()) {
                auto data = response.data();
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
    );
}