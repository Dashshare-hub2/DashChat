#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include "ChatCell.hpp"

using namespace geode::prelude;

class ChatOverlay : public CCNode {
protected:
    CCArray* m_messages = nullptr;
    CCMenu* m_container = nullptr;
    TextInput* m_inputField = nullptr;
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