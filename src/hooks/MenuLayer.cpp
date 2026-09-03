#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../UI/CreateRoomPopup.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu) {
            auto chatIcon = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
            if (!chatIcon) {
                chatIcon = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            }

            auto chatBtn = CCMenuItemSpriteExtra::create(
                chatIcon,
                this,
                menu_selector(MyMenuLayer::onOpenCenterChat)
            );
            chatBtn->setID("dashchat-center-button"_spr);

            bottomMenu->addChild(chatBtn);
            bottomMenu->updateLayout();
        }

        return true;
    }

    void onOpenCenterChat(CCObject* sender) {
        CreateRoomPopup::create()->show();
    }
};