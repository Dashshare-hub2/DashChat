#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class CreateRoomPopup : public geode::Popup<> {
protected:
    bool setup() override;

    geode::EventListener<geode::utils::web::WebTask> m_createRoomTask;

public:
    static CreateRoomPopup* create();
};