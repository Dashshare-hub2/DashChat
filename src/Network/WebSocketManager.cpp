#include "WebSocketManager.hpp"
#include "../UI/ChatOverlay.hpp"

WebSocketManager& WebSocketManager::get() {
    static WebSocketManager instance;
    return instance;
}

void WebSocketManager::connect() {
    std::string serverUrl = Mod::get()->getSettingValue<std::string>("server-url");
    std::string inviteCode = Mod::get()->getSettingValue<std::string>("invite-code");

    m_webSocket.setUrl(serverUrl);

    m_webSocket.setOnMessageCallback([this, inviteCode](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            m_isConnected = true;
            
            // Lấy thông tin tài khoản GD
            auto AM = GJAccountManager::sharedState();
            int accountID = AM->m_accountID;
            std::string username = AM->m_username.empty() ? "Player" : AM->m_username;

            matjson::Value authPayload;
            authPayload["type"] = "auth";
            authPayload["accountID"] = accountID;
            authPayload["username"] = username;
            authPayload["inviteCode"] = inviteCode;

            m_webSocket.send(authPayload.dump());
        } 
        else if (msg->type == ix::WebSocketMessageType::Message) {
            std::string err;
            auto data = matjson::parse(msg->str, err);
            if (err.empty() && data.is_object()) {
                std::string type = data["type"].as_string();

                if (type == "chat") {
                    std::string sender = data["sender"].as_string();
                    std::string message = data["message"].as_string();

                    geode::queueInMainThread([sender, message]() {
                        if (auto overlay = ChatOverlay::get()) {
                            overlay->addMessage(sender, message);
                        }
                    });
                }
                else if (type == "error") {
                    std::string errorMsg = data["message"].as_string();
                    geode::queueInMainThread([errorMsg]() {
                        Notification::create(errorMsg, NotificationIcon::Error)->show();
                    });
                }
                else if (type == "auth_success") {
                    geode::queueInMainThread([]() {
                        Notification::create("DashChat: Ket noi server thanh cong!", NotificationIcon::Success)->show();
                    });
                }
            }
        }
        else if (msg->type == ix::WebSocketMessageType::Close || msg->type == ix::WebSocketMessageType::Error) {
            m_isConnected = false;
        }
    });

    m_webSocket.start();
}

void WebSocketManager::disconnect() {
    m_webSocket.stop();
    m_isConnected = false;
}

void WebSocketManager::sendMessage(const std::string& message) {
    if (!m_isConnected) {
        Notification::create("Chua ket noi toi Server DashChat!", NotificationIcon::Warning)->show();
        return;
    }

    auto AM = GJAccountManager::sharedState();

    matjson::Value chatPayload;
    chatPayload["type"] = "chat";
    chatPayload["sender"] = AM->m_username.empty() ? "Player" : AM->m_username;
    chatPayload["message"] = message;

    m_webSocket.send(chatPayload.dump());
}