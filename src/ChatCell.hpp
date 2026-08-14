#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
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
        discordSprite->setScale(32.f / discordSprite->getContentSize().width);
        discordSprite->setPosition({16.f, 16.f});
        avatarNode->addChild(discordSprite);

        auto gdIcon = SimplePlayer::create(1);
        gdIcon->updatePlayerFrame(1, IconType::Cube);
        gdIcon->setScale(0.35f);
        gdIcon->setPosition({26.f, 6.f}); 
        avatarNode->addChild(gdIcon, 10);

        avatarNode->setPosition({20.f, 20.f});
        this->addChild(avatarNode);

        std::string formattedText = fmt::format("{} - {}: {}", 
            msg.discordName.empty() ? "Guest" : msg.discordName, 
            msg.gdUser, 
            msg.message
        );

        auto label = CCLabelBMFont::create(formattedText.c_str(), "chatFont.fnt");
        label->setAnchorPoint({0.f, 0.5f});
        label->setScale(0.45f);
        label->setPosition({42.f, 20.f});
        this->addChild(label);

        return true;
    }
};