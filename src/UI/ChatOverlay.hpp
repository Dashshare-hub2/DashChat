#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class ChatOverlay : public geode::Popup<std::string const&, bool> {
private:
    std::string m_roomName;
    bool m_readOnly;
    geode::TextInput* m_messageInput = nullptr;
    cocos2d::CCNode* m_chatContainer = nullptr;

    bool setup(std::string const& roomName, bool readOnly) override;
    void onSend(cocos2d::CCObject* sender);
    void onOpenSettings(cocos2d::CCObject* sender);

public:
    static ChatOverlay* create(std::string const& roomName, bool readOnly);
};