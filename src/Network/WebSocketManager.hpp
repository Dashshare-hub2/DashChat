#pragma once
#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <functional>

using MessageCallback = std::function<void(std::string const& sender, std::string const& msg, std::string const& color)>;

class WebSocketManager {
private:
    ix::WebSocket m_webSocket;
    bool m_connected = false;
    MessageCallback m_onMessageReceived;

    WebSocketManager() = default;

public:
    static WebSocketManager& get() {
        static WebSocketManager instance;
        return instance;
    }

    void connect();
    void disconnect();
    void sendMessage(std::string const& text, std::string const& levelID);
    void setOnMessage(MessageCallback cb) { m_onMessageReceived = cb; }
    bool isConnected() const { return m_connected; }
};