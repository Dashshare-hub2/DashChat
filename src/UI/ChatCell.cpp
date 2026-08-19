#include "ChatCell.hpp"

ChatCell* ChatCell::create(const std::string& sender, const std::string& message) {
    auto ret = new ChatCell();
    if (ret && ret->init(sender, message)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(const std::string& sender, const std::string& message) {
    if (!CCNode::init()) return false;

    std::string text = fmt::format("{}: {}", sender, message);
    auto label = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    label->setScale(0.4f);
    label->setAnchorPoint({0.0f, 0.5f});
    this->addChild(label);

    this->setContentSize({ 180.0f, 18.0f });
    return true;
}