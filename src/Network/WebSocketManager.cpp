#include "WebSocketManager.hpp"

using namespace geode::prelude;

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string url = Mod::get()->getSettingValue<std::string>("server-url");
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    if (url.empty()) {
        Notification::create("DashChat: Server URL is empty!", NotificationIcon::Error)->show();
        return;
    }

    std::string fullUrl = url + "?token=" + token;
    m_webSocket.setUrl(fullUrl);

    m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            geode::queueInMainThread([]() {
                Notification::create("DashChat Connected!", NotificationIcon::Success)->show();
            });
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            m_connected = false;
            geode::queueInMainThread([]() {
                Notification::create("DashChat Disconnected!", NotificationIcon::Warning)->show();
            });
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            geode::queueInMainThread([msg]() {
                Notification::create("DashChat Connection Failed!", NotificationIcon::Error)->show();
            });
        } else if (msg->type == ix::WebSocketMessageType::Message) {
            auto jsonResult = matjson::parse(msg->str);
            if (jsonResult.is_ok()) {
                auto data = jsonResult.unwrap();
                std::string sender = data["sender"].as_string();
                std::string text = data["text"].as_string();
                std::string color = data.contains("color") ? data["color"].as_string() : "#FFFFFF";

                geode::queueInMainThread([this, sender, text, color]() {
                    if (m_onMessageReceived) {
                        m_onMessageReceived(sender, text, color);
                    }
                });
            }
        }
    });

    m_webSocket.start();
}

void WebSocketManager::disconnect() {
    m_webSocket.stop();
    m_connected = false;
}

void WebSocketManager::sendMessage(std::string const& text, std::string const& levelID) {
    if (!m_connected) {
        Notification::create("Not connected to DashChat server!", NotificationIcon::Error)->show();
        return;
    }

    matjson::Value json;
    json["type"] = "chat";
    json["text"] = text;
    json["levelID"] = levelID;

    m_webSocket.send(json.dump());
}