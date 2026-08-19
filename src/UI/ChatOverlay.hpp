#pragma once
#include <Geode/Geode.hpp>
#include "ChatCell.hpp"

using namespace geode::prelude;

class ChatOverlay : public CCNode, public CCTextInputDelegate {
protected:
    CCArray* m_messages;
    CCMenu* m_container;
    CCTextInputNode* m_inputField;
    bool m_isTyping = false;
    std::string m_levelID;

    bool init(std::string const& levelID);

public:
    static ChatOverlay* create(std::string const& levelID);
    void addMessage(std::string const& username, std::string const& message, ccColor3B color = {255, 255, 255});
    void updateLayout();
    void toggleInput(bool enable);
    bool isTyping() const { return m_isTyping; }
    void sendCurrentMessage();
};