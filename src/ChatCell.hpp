#pragma once

#include <Geode/Geode.hpp>
#include "ChatManager.hpp"

using namespace geode::prelude;

class ChatCell : public CCNode {
public:
    static ChatCell* create(const ChatMessage& msg, float width) {
        auto ret = new ChatCell();
        if (ret && ret->init(msg, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(const ChatMessage& msg, float width) {
        if (!CCNode::init()) return false;
        this->setContentSize({width, 40.f});

        auto avatarNode = CCNode::create();
        avatarNode->setContentSize({32.f, 32.f});

        auto discordSprite = CCSprite::create("discord_placeholder.png");
        if (!discordSprite) {
            discordSprite = CCSprite::createWithSpriteFrameName("GJ_discordIcon_001.png");
        }
        if (discordSprite) {
            discordSprite->setScale(32.f / discordSprite->getContentSize().width);
            discordSprite->setPosition({16.f, 16.f});
            avatarNode->addChild(discordSprite);
        }


        auto gdIcon = SimplePlayer::create(1);
        if (gdIcon) {
            gdIcon->updatePlayerFrame(1, IconType::Cube);
            gdIcon->setScale(0.35f);
            gdIcon->setPosition({26.f, 6.f});
            avatarNode->addChild(gdIcon);
        }

        avatarNode->setPosition({20.f, 20.f});
        this->addChild(avatarNode);

        std::string userText = msg.author + ": ";
        auto nameLabel = CCLabelBMFont::create(userText.c_str(), "chatFont.fnt");
        if (!nameLabel) nameLabel = CCLabelBMFont::create(userText.c_str(), "bigFont.fnt");
        nameLabel->setScale(0.4f);
        nameLabel->setAnchorPoint({0.f, 0.5f});
        nameLabel->setPosition({42.f, 25.f});
        nameLabel->setColor({255, 200, 100});
        this->addChild(nameLabel);

        auto msgLabel = CCLabelBMFont::create(msg.content.c_str(), "chatFont.fnt");
        if (!msgLabel) msgLabel = CCLabelBMFont::create(msg.content.c_str(), "goldFont.fnt");
        msgLabel->setScale(0.35f);
        msgLabel->setAnchorPoint({0.f, 0.5f});
        msgLabel->setPosition({42.f, 10.f});
        this->addChild(msgLabel);

        return true;
    }
};