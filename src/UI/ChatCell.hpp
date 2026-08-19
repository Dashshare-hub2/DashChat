#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChatCell : public CCNode {
protected:
    CCLabelBMFont* m_usernameLabel;
    CCLabelBMFont* m_messageLabel;

    bool init(std::string const& username, std::string const& message, ccColor3B userColor);

public:
    static ChatCell* create(std::string const& username, std::string const& message, ccColor3B userColor = {255, 255, 255});
};