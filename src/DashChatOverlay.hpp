#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class ChatCell : public CCLayer {
public:
    static ChatCell* create(std::string_view sender, std::string_view message, CCSize size);
    bool init(std::string_view sender, std::string_view message, CCSize size);
};

class DashChatOverlay : public CCLayer {
private:
    ScrollLayer* m_scrollLayer = nullptr;
    CCMenu* m_inputMenu = nullptr;
    CCTextInputNode* m_inputBox = nullptr;
    bool m_isExpanded = false;

public:
    static DashChatOverlay* create();
    bool init() override;

    void addChatMessage(const std::string& sender, const std::string& message);
    void toggleChatInput(bool show);
    void onSend(CCObject* sender);
};