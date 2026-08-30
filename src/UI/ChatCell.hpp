#pragma once

#include <Geode/Geode.hpp>

class ChatCell : public cocos2d::CCNode {
private:
    cocos2d::CCLabelBMFont* m_senderLabel = nullptr;
    cocos2d::CCLabelBMFont* m_messageLabel = nullptr;
    cocos2d::CCSprite* m_avatarSprite = nullptr;

    bool init(std::string const& sender, std::string const& text, cocos2d::ccColor3B color);

public:
    static ChatCell* create(std::string const& sender, std::string const& text, cocos2d::ccColor3B color = { 255, 255, 255 });
    void loadDiscordAvatar(ChatCell* cell, std::string const& avatarUrl);
};