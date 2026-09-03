#include "ChatCell.hpp"

ChatCell* ChatCell::create() {
    auto ret = new ChatCell();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init() {
    if (!CCNode::init()) return false;
    this->setContentSize({350.0f, 35.0f});
    return true;
}