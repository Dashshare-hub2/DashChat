#include "WebSocketManager.hpp"
#include <Geode/Geode.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

WebSocketManager& WebSocketManager::get() {
    static WebSocketManager instance;
    return instance;
}

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string baseUrl = Mod::get()->getSettingValue<std::string>("server-url");
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    if (baseUrl.empty()) {
        baseUrl = "wss://dashchat-rsuk.onrender.com";
    }

    m_webSocket.setUrl(baseUrl);

    m_webSocket.setOnMessageCallback([this](ix::WebSocketMessageType type, const std::string& str, size_t, const ix::WebSocketErrorInfo& errorInfo, size_t) {
        if (type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            geode::Loader::get()->queueInMainThread([]() {
                Notification::create("DashChat Connected!", NotificationIcon::Success)->show();
            });
        } 
        else if (type == ix::WebSocketMessageType::Close || type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            std::string reason = errorInfo.reason.empty() ? "Disconnected" : errorInfo.reason;
            geode::Loader::get()->queueInMainThread([reason]() {
                Notification::create("DashChat Err: " + reason, NotificationIcon::Error)->show();
            });
        } 
        else if (type == ix::WebSocketMessageType::Message) {
            auto parseResult = matjson::parse(str);
            if (parseResult.isOk()) {
                auto json = parseResult.unwrap();
                std::string sender = json["sender"].asString().unwrapOrDefault("Unknown");
                std::string text = json["text"].asString().unwrapOrDefault("");
                std::string color = json["color"].asString().unwrapOrDefault("#FFFFFF");

                if (m_onMessageReceived) {
                    geode::Loader::get()->queueInMainThread([this, sender, text, color]() {
                        m_onMessageReceived(sender, text, color);
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
    if (!m_connected || text.empty()) return;

    matjson::Value json = matjson::Object {
        { "text", text }
    };

    m_webSocket.send(json.dump());
}