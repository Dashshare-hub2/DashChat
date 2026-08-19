#include "ChatLayer.hpp"
#include "ChatCell.hpp"

bool ChatLayer::setup(std::string const& inviteCode) {
    m_inviteCode = inviteCode;
    this->setTitle("DashChat");

    auto winSize = m_mainLayer->getContentSize();

    m_scrollLayer = ScrollLayer::create({winSize.width - 20.f, winSize.height - 60.f});
    m_scrollLayer->setPosition({10.f, 40.f});
    m_mainLayer->addChild(m_scrollLayer);

    this->reloadMessages();

    return true;
}

void ChatLayer::reloadMessages() {
    if (!m_scrollLayer) return;

    m_scrollLayer->m_contentLayer->removeAllChildren();

    auto messages = ChatManager::get()->getMessages();
    float width = m_scrollLayer->getContentSize().width;
    float totalHeight = messages.size() * 42.f;

    m_scrollLayer->m_contentLayer->setContentSize({width, std::max(totalHeight, m_scrollLayer->getContentSize().height)});

    float y = totalHeight - 20.f;
    for (const auto& msg : messages) {
        auto cell = ChatCell::create(msg, width);
        cell->setPosition({0.f, y - 20.f});
        m_scrollLayer->m_contentLayer->addChild(cell);
        y -= 42.f;
    }

    m_scrollLayer->moveToTop();
}

ChatLayer* ChatLayer::create(std::string const& inviteCode) {
    auto ret = new ChatLayer();
    if (ret && ret->initAnchored(360.f, 220.f, inviteCode)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}