#pragma once
#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>

using namespace geode::prelude;

class WebSocketManager {
private:
    ix::WebSocket m_webSocket;
    bool m_isConnected = false;

    WebSocketManager() = default;

public:
    static WebSocketManager& get();

    void connect();
    void disconnect();
    void sendMessage(const std::string& message);
    bool isConnected() const { return m_isConnected; }
};