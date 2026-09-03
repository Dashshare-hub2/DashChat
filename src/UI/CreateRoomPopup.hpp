#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/utils/web.hpp>

class CreateRoomPopup : public geode::Popup<> {
private:
    geode::TextInput* m_roomNameInput = nullptr;
    geode::TextInput* m_passwordInput = nullptr;
    geode::EventListener<geode::utils::web::WebTask> m_createRoomTask;

    bool setup() override;
    void onCreateRoom(cocos2d::CCObject* sender);

public:
    static CreateRoomPopup* create();
};