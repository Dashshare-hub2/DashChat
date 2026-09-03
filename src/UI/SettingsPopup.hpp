#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class SettingsPopup : public geode::Popup<> {
private:
    geode::TextInput* m_serverInput = nullptr;
    geode::TextInput* m_firebaseInput = nullptr;

    bool setup() override;
    void onSaveAndConnect(cocos2d::CCObject* sender);
    void onLinkDiscord(cocos2d::CCObject* sender);

public:
    static SettingsPopup* create();
};