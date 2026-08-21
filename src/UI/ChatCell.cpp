#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

void loadDiscordAvatar(CCNode* parentNode, std::string const& avatarUrl) {
    if (avatarUrl.empty() || !parentNode) return;

    auto listener = new Task<web::WebResponse>::Listener();

    listener->bind([parentNode, listener](Task<web::WebResponse>::Event* event) {
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
            delete listener;
        } else if (event->isCancelled()) {
            delete listener;
        }
    });

    web::WebRequest req;
    listener->setFilter(req.get(avatarUrl));
}