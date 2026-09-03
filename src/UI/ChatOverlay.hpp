#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ChatOverlay : public geode::Popup<std::string const&, bool> {
protected:
    bool setup(std::string const& roomName, bool readOnly) override;

public:
    static ChatOverlay* create(std::string const& roomName, bool readOnly);
};