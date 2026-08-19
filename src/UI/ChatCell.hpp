#pragma once
#include <Geode/Geode.hpp>

class ChatCell : public cocos2d::CCNode {
private:
    cocos2d::CCLayerColor* m_bg = nullptr;
    cocos2d::CCLabelBMFont* m_label = nullptr;

public:
    static ChatCell* create(std::string const& sender, std::string const& text, cocos2d::ccColor3B color);
    bool init(std::string const& sender, std::string const& text, cocos2d::ccColor3B color);
};