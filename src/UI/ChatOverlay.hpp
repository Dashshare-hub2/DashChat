#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

class ChatOverlay : public geode::Popup<std::string const&, bool> {
protected:
    std::string m_roomName;
    bool m_readOnly;

    bool setup(std::string const& roomName, bool readOnly) override;
    void onSend(cocos2d::CCObject* sender);

public:
    static ChatOverlay* create(std::string const& roomName, bool readOnly);
};