#pragma once
#include <Geode/Geode.hpp>
#include "ChatCell.hpp"

class ChatOverlay : public cocos2d::CCNode {
private:
    cocos2d::CCArray* m_messages = nullptr;
    cocos2d::CCMenu* m_container = nullptr;
    geode::TextInput* m_inputField = nullptr;
    bool m_isTyping = false;

public:
    static ChatOverlay* create(std::string const& levelID);
    bool init(std::string const& levelID);
    void addMessage(std::string const& username, std::string const& message, cocos2d::ccColor3B color);
    void updateLayout();
    void toggleTyping(bool typing);
    void sendMessage();
    bool isTyping() const;

    ~ChatOverlay();
};
