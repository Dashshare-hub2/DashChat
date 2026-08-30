#include "WebSocketManager.hpp"
#include <matjson.hpp>

using namespace geode::prelude;

WebSocketManager& WebSocketManager::get() {
    static WebSocketManager instance;
    return instance;
}

WebSocketManager::WebSocketManager() {
    CCScheduler::sharedScheduler()->scheduleSelector(
        schedule_selector(WebSocketManager::updateDispatch),
        this, 0.1f, false
    );
}

WebSocketManager::~WebSocketManager() {
    disconnect();
}

void WebSocketManager::connect(std::string const& url) {
    if (m_isConnected) return;

    m_webSocket.setUrl(url);
    m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            std::string sender = "User";
            std::string text = msg->str;
            std::string avatarUrl = "";

            auto parseRes = matjson::parse(msg->str);
            if (parseRes.is_ok()) {
                auto json = parseRes.unwrap();
                if (json.contains("sender")) sender = json["sender"].as_string();
                if (json.contains("text")) text = json["text"].as_string();
                if (json.contains("avatar")) avatarUrl = json["avatar"].as_string();
            }

            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_messageQueue.push({ sender, text, avatarUrl });
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            m_isConnected = true;
        } else if (msg->type == ix::WebSocketMessageType::Close || msg->type == ix::WebSocketMessageType::Error) {
            m_isConnected = false;
        }
    });

    m_webSocket.start();
}

void WebSocketManager::disconnect() {
    if (m_isConnected) {
        m_webSocket.stop();
        m_isConnected = false;
    }
}

void WebSocketManager::send(std::string const& text) {
    if (m_isConnected) {
        m_webSocket.send(text);
    }
}

void WebSocketManager::setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback) {
    m_onMessageCallback = callback;
}

void WebSocketManager::updateUI() {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    while (!m_messageQueue.empty()) {
        auto msg = m_messageQueue.front();
        m_messageQueue.pop();
        if (m_onMessageCallback) {
            m_onMessageCallback(msg.sender, msg.text, msg.avatarUrl);
        }
    }
}

void WebSocketManager::updateDispatch(float dt) {
    this->updateUI();
}