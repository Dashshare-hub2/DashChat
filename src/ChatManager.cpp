#include "ChatManager.hpp"
#include <fmt/format.h>

using namespace geode::prelude;

ChatManager* ChatManager::get() {
    static ChatManager instance;
    return &instance;
}

void ChatManager::connect() {
    bool enableChat = Mod::get()->getSettingValue<bool>("enable-chat");
    if (!enableChat) {
        log::info("[DashChat] Chat disconnected");
        disconnect();
        return;
    }


    std::string serverUrl = Mod::get()->getSettingValue<std::string>("server-url");
    std::string inviteCode = Mod::get()->getSettingValue<std::string>("invite-code");

    auto am = GJAccountManager::sharedState();
    std::string username = am->m_username.empty() ? "Player" : am->m_username;
    int accountID = am->m_accountID;

    if (serverUrl.empty()) {
        log::error("[DashChat] empty server");
        return;
    }

 
    disconnect();

  
    std::string fullUrl = fmt::format("{}/chat?room={}&user={}&account_id={}", 
        serverUrl, inviteCode, username, accountID
    );

    log::info("[DashChat] Connecting: {}", fullUrl);
    m_webSocket.setUrl(fullUrl);

    m_webSocket.setOnMessageCallback([this](const ixwebsocket::WebSocketMessagePtr& msg) {
        if (msg->type == ixwebsocket::WebSocketMessageType::Message) {
            std::string payload = msg->str;
            log::info("[DashChat] message: {}", payload);

            Loader::get()->queueInMainThread([this, payload]() {
                std::lock_guard<std::mutex> lock(m_callbackMutex);
                if (m_onMessageCallback) {
                    size_t pos = payload.find(": ");
                    if (pos != std::string::npos) {
                        std::string sender = payload.substr(0, pos);
                        std::string content = payload.substr(pos + 2);
                        bool isDiscord = (sender.find("[Discord]") != std::string::npos);
                        m_onMessageCallback(sender, content, isDiscord);
                    } else {
                        m_onMessageCallback("System", payload, false);
                    }
                }
            });
        } 
        else if (msg->type == ixwebsocket::WebSocketMessageType::Open) {
            m_isConnected = true;
            log::info("[DashChat] connected!");
        } 
        else if (msg->type == ixwebsocket::WebSocketMessageType::Close || 
                 msg->type == ixwebsocket::WebSocketMessageType::Error) {
            m_isConnected = false;
            log::warn("[DashChat] disconnected.");
        }
    });

    m_webSocket.start();
}

void ChatManager::disconnect() {
    if (m_isConnected) {
        m_webSocket.stop();
        m_isConnected = false;
        log::info("[DashChat] Disconnected to WS.");
    }
}

void ChatManager::sendMessage(const std::string& message) {
    if (m_isConnected && m_webSocket.getReadyState() == ixwebsocket::ReadyState::Open) {
        m_webSocket.send(message);
    } else {
        log::warn("[DashChat] Unable connect!");
    }
}

void ChatManager::setOnMessageCallback(std::function<void(const std::string&, const std::string&, bool)> cb) {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_onMessageCallback = cb;
}