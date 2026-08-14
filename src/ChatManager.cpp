#include "ChatManager.hpp"
#include <ixwebsocket/IXWebSocket.h>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

ChatManager::ChatManager() {
    m_webSocket = std::make_unique<ix::WebSocket>();
}

ChatManager::~ChatManager() {
    if (m_webSocket) {
        m_webSocket->stop();
    }
}

ChatManager* ChatManager::get() {
    static ChatManager instance;
    return &instance;
}

void ChatManager::connect() {
    bool enabled = Mod::get()->getSettingValue<bool>("enable-chat");
    if (!enabled || m_connected) return;

    std::string serverUrl = Mod::get()->getSettingValue<std::string>("server-url");
    m_webSocket->setUrl(serverUrl);

    m_webSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            log::info("Connected to DashChat server!");
            
            // Lấy Invite Code từ Mod Settings và tự động tham gia
            std::string code = Mod::get()->getSettingValue<std::string>("invite-code");
            joinWithInvite(code);
        } 
        else if (msg->type == ix::WebSocketMessageType::Close || msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            m_joined = false;
            log::error("WebSocket Error/Close: {}", msg->errorInfo.reason);
        } 
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto parseResult = matjson::parse(msg->str);
            if (!parseResult.is_ok()) return;

            auto json = parseResult.unwrap();
            std::string type = json["type"].as_string();

            if (type == "join_success") {
                m_joined = true;
                std::string notice = json["message"].as_string();
                Loader::get()->queueInMainThread([notice]() {
                    Notification::create(notice, NotificationIcon::Success)->show();
                });
            }
            else if (type == "chat_message") {
                std::string sender = json["sender"].as_string();
                std::string discordUser = json.contains("discordUser") && !json["discordUser"].is_null() 
                                         ? json["discordUser"].as_string() : "";
                std::string message = json["message"].as_string();

                Loader::get()->queueInMainThread([this, sender, discordUser, message]() {
                    m_messages.push_back({sender, discordUser, message});
                });
            }
            else if (type == "error") {
                std::string err = json["message"].as_string();
                Loader::get()->queueInMainThread([err]() {
                    Notification::create(err, NotificationIcon::Error)->show();
                });
            }
        }
    });

    m_webSocket->start();
}

void ChatManager::disconnect() {
    if (m_webSocket && m_connected) {
        m_webSocket->stop();
        m_connected = false;
        m_joined = false;
    }
}

void ChatManager::joinWithInvite(const std::string& inviteCode) {
    if (!m_connected) return;

    std::string gdName = GJAccountManager::get()->m_username.empty() 
                         ? "Player" 
                         : GJAccountManager::get()->m_username.c_str();

    matjson::Value json;
    json["type"] = "join_invite";
    json["inviteCode"] = inviteCode;
    json["gdName"] = gdName;

    m_webSocket->send(json.dump());
}

void ChatManager::linkDiscord(const std::string& discordUsername, const std::string& discordId) {
    if (!m_connected) return;

    std::string gdName = GJAccountManager::get()->m_username.c_str();

    matjson::Value json;
    json["type"] = "link_discord";
    json["gdName"] = gdName;
    json["discordUsername"] = discordUsername;
    json["discordId"] = discordId;

    m_webSocket->send(json.dump());
}

void ChatManager::sendMessage(const std::string& msg) {
    if (!m_connected || !m_joined) return;

    matjson::Value json;
    json["type"] = "chat_message";
    json["message"] = msg;

    m_webSocket->send(json.dump());
}