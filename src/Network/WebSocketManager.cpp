#include "WebSocketManager.hpp"
#include <matjson.hpp>

using namespace geode::prelude;

WebSocketManager& WebSocketManager::get() {
    static WebSocketManager instance;
    return instance;
}

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string baseUrl = Mod::get()->getSettingValue<std::string>("server-url");
    std::string jwtToken = Mod::get()->getSettingValue<std::string>("user-token");

    if (baseUrl.empty()) baseUrl = "wss://dashchat-rsuk.onrender.com";
    if (!jwtToken.empty()) {
        baseUrl += "?token=" + jwtToken;
    }

    m_webSocket.setUrl(baseUrl);

    m_webSocket.setOnMessageCallback([this](const std::unique_ptr<ix::WebSocketMessage>& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            geode::Loader::get()->queueInMainThread([]() {
                Notification::create("DashChat Authenticated!", NotificationIcon::Success)->show();
            });
        } 
        else if (msg->type == ix::WebSocketMessageType::Close || msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto parseResult = matjson::parse(msg->str);
            if (parseResult.isOk()) {
                auto json = parseResult.unwrap();
                std::string sender = json["sender"].asString().unwrapOr("Unknown");
                std::string text = json["text"].asString().unwrapOr("");
                std::string avatarUrl = json["avatar"].asString().unwrapOr("");

                if (m_onMessageReceived) {
                    geode::Loader::get()->queueInMainThread([this, sender, text, avatarUrl]() {
                        m_onMessageReceived(sender, text, avatarUrl);
                    });
                }
            }
        }
    });

    m_webSocket.start();
}

void WebSocketManager::setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback) {
    m_onMessageReceived = callback;
}

void WebSocketManager::send(std::string const& text) {
    if (text.empty()) return;

    matjson::Value json;
    json["text"] = text;

    m_webSocket.send(json.dump());
}