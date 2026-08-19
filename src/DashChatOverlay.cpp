#include "DashChatOverlay.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

ChatCell* ChatCell::create(std::string_view sender, std::string_view message, CCSize size) {
    auto ret = new ChatCell();
    if (ret && ret->init(sender, message, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChatCell::init(std::string_view sender, std::string_view message, CCSize size) {
    if (!CCLayer::init()) return false;

    this->setContentSize(size);

    auto bg = CCLayerColor::create({0, 0, 0, 100}, size.width, size.height);
    bg->setIgnoreAnchorPointForPosition(false);
    bg->setAnchorPoint({0, 0});
    this->addChild(bg);

    std::string fullText = fmt::format("{}: {}", sender, message);
    auto label = CCLabelBMFont::create(fullText.c_str(), "chatFont.fnt");
    label->setScale(0.4f);
    label->setAnchorPoint({0.0f, 0.5f});
    label->setPosition({8.0f, size.height / 2.0f});
    
    if (auto senderLabel = dynamic_cast<CCLabelBMFont*>(label)) {
    }

    this->addChild(label);
    return true;
}

DashChatOverlay* DashChatOverlay::create() {
    auto ret = new DashChatOverlay();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DashChatOverlay::init() {
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    CCSize chatSize = { 180.0f, 100.0f };
    this->setContentSize(chatSize);
    this->setPosition({ 10.0f, winSize.height - 110.0f });

    m_scrollLayer = ScrollLayer::create(chatSize);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAlignment(FlexAlignment::Start)
            ->setGap(3.0f)
    );
    this->addChild(m_scrollLayer);

    this->addChatMessage("System", "Welcome to DashChat!");
    this->addChatMessage("Player1", "Good luck on this level!");

    return true;
}

void DashChatOverlay::addChatMessage(const std::string& sender, const std::string& message) {
    if (!m_scrollLayer) return;

    auto cell = ChatCell::create(sender, message, { 180.0f, 20.0f });
    m_scrollLayer->m_contentLayer->addChild(cell);
    m_scrollLayer->m_contentLayer->updateLayout();
    m_scrollLayer->scrollToTop();
}

class $modify(DashPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto chatOverlay = DashChatOverlay::create();
        chatOverlay->setID("dashchat-overlay"_spr);
        this->addChild(chatOverlay, 100);

        return true;
    }
};

class $modify(DashPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("left-button-menu");
        if (!menu) menu = this->getChildByID("center-button-menu");

        if (menu) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "geode.loader/geode-logo-outline.png", 
                0.8f,
                CircleBaseColor::Green,
                CircleBaseSize::Medium
            );

            auto chatBtn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(DashPauseLayer::onToggleDashChat)
            );
            chatBtn->setID("dashchat-pause-button"_spr);

            menu->addChild(chatBtn);
            menu->updateLayout();
        }
    }

    void onToggleDashChat(CCObject* sender) {
        this->onResume(sender);

        FLAlertLayer::create("DashChat", "Mo khung nhap chat!", "OK")->show();
    }
};