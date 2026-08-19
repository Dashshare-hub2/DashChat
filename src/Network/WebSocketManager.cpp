#include "WebSocketManager.hpp"

using namespace geode::prelude;

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string url = Mod::get()->getSettingValue<std::string>("server-url");
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    log::info("DashChat attempting connection to: {}", url);

    if (url.empty()) {
        Loader::get()->queueInMainThread([]() {
            Notification::create("DashChat: URL empty!", NotificationIcon::Error)->show();
        });
        return;
    }

    m_webSocket.setUrl(url + "?token=" + token);

    m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            log::info("DashChat WebSocket Open!");
            Loader::get()->queueInMainThread([]() {
                Notification::create("DashChat Connected!", NotificationIcon::Success)->show();
            });
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            log::error("DashChat Error: {}", msg->errorInfo.reason);
            Loader::get()->queueInMainThread([reason = msg->errorInfo.reason]() {
                Notification::create("DashChat Err: " + reason, NotificationIcon::Error)->show();
            });
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto jsonResult = matjson::parse(msg->str);
            if (jsonResult.is_ok()) {
                auto data = jsonResult.unwrap();
                std::string sender = data["sender"].as_string();
                std::string text = data["text"].as_string();
                std::string color = data.contains("color") ? data["color"].as_string() : "#FFFFFF";

                Loader::get()->queueInMainThread([this, sender, text, color]() {
                    if (m_onMessageReceived) {
                        m_onMessageReceived(sender, text, color);
                    }
                });
            }
        }
    });

    m_webSocket.start();
}