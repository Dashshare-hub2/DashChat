#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>      
#include <Geode/utils/web.hpp>         
#include <Geode/ui/Popup.hpp>


class ChatCell : public cocos2d::CCNode {
private:
    cocos2d::CCSprite* m_avatarSprite = nullptr;
    cocos2d::CCLabelBMFont* m_usernameLabel = nullptr;
    cocos2d::CCLabelBMFont* m_messageLabel = nullptr;

    geode::EventListener<geode::utils::web::WebTask> m_avatarTask;

public:
    static ChatCell* create(
        std::string const& sender, 
        std::string const& text, 
        std::string const& avatarUrl = "", 
        cocos2d::ccColor3B color = { 255, 255, 255 }
    );

    bool init(
        std::string const& sender, 
        std::string const& text, 
        std::string const& avatarUrl, 
        cocos2d::ccColor3B color
    );

    void loadDiscordAvatar(std::string const& avatarUrl);
};
