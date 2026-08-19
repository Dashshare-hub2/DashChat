#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChatCell : public CCNodeRGBA {
protected:
    CCLayerColor* m_bg = nullptr;
    CCLabelBMFont* m_usernameLabel = nullptr;
    CCLabelBMFont* m_messageLabel = nullptr;

    bool init(std::string const& username, std::string const& message, ccColor3B userColor);

public:
    static ChatCell* create(std::string const& username, std::string const& message, ccColor3B userColor = {255, 255, 255});
    void setCellOpacity(GLubyte opacity);
};