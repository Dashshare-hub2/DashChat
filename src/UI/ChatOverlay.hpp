#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

class ChatOverlay : public geode::Popup {
protected:
    std::string m_roomName;
    bool m_readOnly;

    bool setup(std::string const& roomName, bool readOnly);
    void onSend(cocos2d::CCObject* sender);

public:
    static ChatOverlay* create(std::string const& roomName, bool readOnly);
};