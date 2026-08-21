#include "WebSocketManager.hpp"
#include <Geode/Geode.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

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
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto parseResult = matjson::parse(msg->str);
            if (parseResult.isOk()) {
                auto json = parseResult.unwrap();
                std::string sender = json["sender"].asString().unwrapOr("Unknown");
                std::string text = json["text"].asString().unwrapOr("");
                std::string avatarUrl = json["avatar"].asString().unwrapOr(""); // Link Avatar Discord

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