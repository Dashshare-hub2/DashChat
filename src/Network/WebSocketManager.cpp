#include "WebSocketManager.hpp"
#include <matjson.hpp>

WebSocketManager& WebSocketManager::get() {
    static WebSocketManager instance;
    return instance;
}

void WebSocketManager::connect() {
    if (m_connected) return;

    std::string baseUrl = Mod::get()->getSettingValue<std::string>("server-url");
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    if (baseUrl.empty()) return;

    if (baseUrl.back() == '/') baseUrl.pop_back();
    std::string fullUrl = baseUrl + "/?token=" + token;

    m_webSocket.setUrl(fullUrl);

    ix::SocketTLSOptions tlsOptions;
    tlsOptions.tls = true;
    m_webSocket.setTLSOptions(tlsOptions);

    m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_connected = true;
            geode::queueInMainThread([]() {
                Notification::create("DashChat Connected!", NotificationIcon::Success)->show();
            });
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            m_connected = false;
            geode::queueInMainThread([reason = msg->errorInfo.reason]() {
                Notification::create("DashChat Err: " + reason, NotificationIcon::Error)->show();
            });
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto jsonResult = matjson::parse(msg->str);
            if (jsonResult.isOk()) {
                auto data = jsonResult.unwrap();
                std::string sender = data["sender"].asString().unwrapOrDefault();
                std::string text = data["text"].asString().unwrapOrDefault();
                std::string color = data.contains("color") ? data["color"].asString().unwrapOrDefault() : "#FFFFFF";

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

void WebSocketManager::setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback) {
    m_onMessageReceived = callback;
    m_webSocket.send(json.dump());

void WebSocketManager::send(std::string const& text) {
    if (!m_connected || text.empty()) return;

    matjson::Value json = matjson::Object {
        { "text", text }
    };

    m_webSocket.send(json.dump());
}