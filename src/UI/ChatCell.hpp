#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

class ChatCell : public cocos2d::CCNode {
private:
    geode::async::TaskHolder m_avatarListener;

public:
    static ChatCell* create(std::string const& sender, std::string const& text, cocos2d::ccColor3B color);
    bool init(std::string const& sender, std::string const& text, cocos2d::ccColor3B color);
    void loadDiscordAvatar(cocos2d::CCNode* parentNode, std::string const& avatarUrl);
};