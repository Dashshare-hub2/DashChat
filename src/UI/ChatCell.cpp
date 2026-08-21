#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

void loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    geode::async::spawn([parentNode, avatarUrl]() -> geode::async::Task<void> {
        web::WebRequest req;
        auto res = co_await req.get(avatarUrl);
        
        if (res.ok()) {
            auto data = res.data();
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
}