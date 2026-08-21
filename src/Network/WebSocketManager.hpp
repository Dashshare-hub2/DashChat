#pragma once
#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include <functional>
#include <string>

class WebSocketManager {
private:
    ix::WebSocket m_webSocket;
    bool m_connected = false;
    std::function<void(std::string const&, std::string const&, std::string const&)> m_onMessageReceived;

    WebSocketManager() = default;

public:
    static WebSocketManager& get();

    void connect();
    void send(std::string const& text);
    void setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback);
};