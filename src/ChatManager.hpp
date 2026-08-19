#pragma once

#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <functional>
#include <mutex>

class ChatManager {
private:
    ixwebsocket::WebSocket m_webSocket;
    bool m_isConnected = false;
    std::mutex m_callbackMutex;
    
    std::function<void(const std::string&, const std::string&, bool)> m_onMessageCallback;

    ChatManager() = default;

public:
    static ChatManager* get();

    void connect();
    void disconnect();
    void sendMessage(const std::string& message);
    
    void setOnMessageCallback(std::function<void(const std::string&, const std::string&, bool)> cb);
    bool isConnected() const { return m_isConnected; }
};