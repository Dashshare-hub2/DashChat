#include "WebSocketManager.hpp"

using namespace geode::prelude;

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string url = Mod::get()->getSettingValue<std::string>("server-url");
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    log::info("DashChat connecting to URL: {}", url);

    if (url.empty()) {
        Loader::get()->queueInMainThread([]() {
            Notification::create("DashChat: URL is empty!", NotificationIcon::Error)->show();
        });
        return;
    }

    std::string fullUrl = url + "?token=" + token;
    m_webSocket.setUrl(fullUrl);

    m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            log::info("DashChat WebSocket Open!");

            Loader::get()->queueInMainThread([]() {
                Notification::create("DashChat Connected!", NotificationIcon::Success)->show();
            });
        } 
        else if (msg->type == ix::WebSocketMessageType::Close) {
            m_connected = false;
            log::warn("DashChat WebSocket Closed: {}", msg->closeInfo.code);

            Loader::get()->queueInMainThread([code = msg->closeInfo.code]() {
                std::string errStr = "DashChat Closed: " + std::to_string(code);
                Notification::create(errStr, NotificationIcon::Warning)->show();
            });
        } 
        else if (msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            log::error("DashChat WebSocket Error: {}", msg->errorInfo.reason);

            Loader::get()->queueInMainThread([reason = msg->errorInfo.reason]() {
                std::string errStr = "DashChat Error: " + reason;
                Notification::create(errStr, NotificationIcon::Error)->show();
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

void WebSocketManager::disconnect() {
    m_webSocket.stop();
    m_connected = false;
}

void WebSocketManager::sendMessage(std::string const& text, std::string const& levelID) {
    if (!m_connected) {
        Loader::get()->queueInMainThread([]() {
            Notification::create("Cannot send: Disconnected!", NotificationIcon::Error)->show();
        });
        return;
    }

    matjson::Value json;
    json["type"] = "chat";
    json["text"] = text;
    json["levelID"] = levelID;

    m_webSocket.send(json.dump());
}