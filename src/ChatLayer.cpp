#include "ChatLayer.hpp"
#include "ChatManager.hpp"

using namespace geode::prelude;

bool ChatLayer::setup(std::string const& roomName) {
    this->setTitle("DashChat - Room: " + roomName);

    auto winSize = m_mainLayer->getContentSize();


    m_messagesMenu = CCMenu::create();
    m_messagesMenu->setPosition({0, 0});
    m_messagesMenu->setContentSize({winSize.width - 40.f, 130.f});


    m_inputNode = CCTextInputNode::create(200.f, 30.f, "Nhập tin nhắn...", "chatFont.fnt");
    m_inputNode->setPosition({winSize.width / 2.0f - 30.f, 30.f});
    m_mainLayer->addChild(m_inputNode);


    auto sendSpr = ButtonSprite::create("Gửi", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto sendBtn = CCMenuItemSpriteExtra::create(
        sendSpr, this, menu_selector(ChatLayer::onSend)
    );
    sendBtn->setPosition({winSize.width / 2.0f + 90.f, 30.f});

    auto actionMenu = CCMenu::create();
    actionMenu->setPosition({0, 0});
    actionMenu->addChild(sendBtn);
    m_mainLayer->addChild(actionMenu);


    ChatManager::get()->setOnMessageCallback([this](const std::string& sender, const std::string& msg, bool isDiscord) {
        this->appendMessageUI(sender, msg, isDiscord);
    });

    return true;
}

void ChatLayer::onSend(CCObject*) {
    if (!m_inputNode) return;

    std::string text = m_inputNode->getString();
    if (!text.empty()) {
 
        ChatManager::get()->sendMessage(text);

  
        auto username = GJAccountManager::sharedState()->m_username;
        appendMessageUI(username.empty() ? "Me" : username, text, false);

        m_inputNode->setString("");
    }
}

void ChatLayer::appendMessageUI(const std::string& sender, const std::string& content, bool isDiscord) {
    std::string formattedText = fmt::format("{}{}: {}", isDiscord ? "[Discord] " : "", sender, content);
    
    auto label = CCLabelBMFont::create(formattedText.c_str(), "chatFont.fnt");
    label->setScale(0.55f);
    label->setAnchorPoint({0.0f, 0.5f});
    
    if (isDiscord) {
        label->setColor({114, 137, 218});
    }

    label->setPosition({20.f, m_mainLayer->getContentSize().height - 70.f - m_chatContentHeight});
    m_mainLayer->addChild(label);

    m_chatContentHeight += 16.0f; 
}

ChatLayer* ChatLayer::create(std::string const& roomName) {
    auto ret = new ChatLayer();
    if (ret && ret->initAnchored(340.f, 220.f, roomName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ChatLayer::~ChatLayer() {
    ChatManager::get()->setOnMessageCallback(nullptr);
}