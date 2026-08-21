#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

void loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    web::WebRequest req;
    req.get(avatarUrl).listen([parentNode](web::WebResponse* response) {
        if (response && response->ok()) {
            auto data = response->data();

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
    });
}