#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

#include <Geode/cocos/extensions/GUI/CCScrollView/CCScrollView.h>

class ChatOverlay : public geode::Popup<std::string const&> {
private:
    cocos2d::CCNode* m_chatContainer = nullptr;
    geode::TextInput* m_inputNode = nullptr;
    cocos2d::extension::CCScrollView* m_scrollView = nullptr;
    float m_chatHeight = 0.0f;

    bool setup(std::string const& roomName) override;
    void onSend(cocos2d::CCObject* sender);

public:
    static ChatOverlay* create(std::string const& roomName);
    void addChatMessage(std::string const& sender, std::string const& text, std::string const& avatarUrl);
}; 