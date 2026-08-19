#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class ChatLayer : public Popup<std::string const&> {
protected:
    std::string m_inviteCode;
    ScrollLayer* m_scrollLayer = nullptr;

    bool setup(std::string const& inviteCode) override;

public:
    static ChatLayer* create(std::string const& inviteCode);
    void reloadMessages();
};