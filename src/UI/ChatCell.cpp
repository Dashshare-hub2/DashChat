#include "ChatCell.hpp"

using namespace geode::prelude;

ChatCell* ChatCell::create(std::string const& sender, std::string const& text, ccColor3B color) {
    auto ret = new ChatCell();
    if (ret && ret->init(sender, text, color)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(std::string const& sender, std::string const& text, ccColor3B color) {
    if (!CCNode::init()) return false;

    m_bg = CCLayerColor::create(ccc4(0, 0, 0, 100), 280.0f, 20.0f);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({0.0f, 0.0f});
    this->addChild(m_bg);

    std::string formatted = sender + ": " + text;
    m_label = CCLabelBMFont::create(formatted.c_str(), "chatFont.fnt");
    m_label->setColor(color);
    m_label->setAnchorPoint({0.0f, 0.5f});
    m_label->setPosition({5.0f, 10.0f});
    m_label->setScale(0.5f);
    this->addChild(m_label);

    this->setContentSize({280.0f, 20.0f});
    return true;
}