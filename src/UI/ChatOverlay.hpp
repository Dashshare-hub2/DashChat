#pragma once
#include <Geode/Geode.hpp>

class ChatOverlay : public cocos2d::CCNode {
private:
    bool m_isTyping = false;
    geode::TextInput* m_inputField = nullptr;

public:
    static ChatOverlay* create(std::string const& levelID);
    
    bool init(std::string const& levelID);
    void toggleTyping(bool typing);
    void sendMessage();
    bool isTyping() const { return m_isTyping; }
};