#pragma once
#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>

class WebSocketManager {
private:
    ix::WebSocket m_webSocket;
    bool m_connected = false;
    std::function<void(std::string const&, std::string const&, std::string const&)> m_onMessageReceived;

    WebSocketManager() = default;

public:
    static WebSocketManager& get();
    void connect();
    void setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback);
};