#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ChatLayer : public Popup<std::string const&> {
protected:
    CCTextInputNode* m_inputNode = nullptr;
    CCScrollLayer* m_scrollLayer = nullptr;
    CCMenu* m_messagesMenu = nullptr;
    float m_chatContentHeight = 10.0f;

    bool setup(std::string const& roomName) override;
    void onSend(CCObject* sender);
    void appendMessageUI(const std::string& sender, const std::string& content, bool isDiscord);

public:
    static ChatLayer* create(std::string const& roomName);
    ~ChatLayer();
};