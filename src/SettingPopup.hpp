#include <Geode/Geode.hpp>
#include "ChatManager.hpp"

using namespace geode::prelude;

class SettingsPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("DashChat Settings & Discord");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto linkBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Link Discord"),
            this,
            menu_selector(SettingsPopup::onLinkDiscord)
        );
        linkBtn->setPosition({0, 20});

        auto geodeSettingsBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Server Settings"),
            this,
            menu_selector(SettingsPopup::onOpenGeodeSettings)
        );
        geodeSettingsBtn->setPosition({0, -30});

        this->m_buttonMenu->addChild(linkBtn);
        this->m_buttonMenu->addChild(geodeSettingsBtn);

        return true;
    }

    void onLinkDiscord(CCObject*) {
        geode::utils::web::openLinkInBrowser("https://discord.com/api/oauth2/authorize?client_id=1536353977304621077&response_type=code&scope=identify");

    }

    void onOpenGeodeSettings(CCObject*) {
        geode::openSettingsPopup(Mod::get());
    }

public:
    static SettingsPopup* create() {
        auto ret = new SettingsPopup();
        if (ret && ret->initAnchored(300, 200)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};