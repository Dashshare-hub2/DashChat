#pragma once

#include <Geode/Geode.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include <functional>
#include <string>
#include <mutex>
#include <queue>

class WebSocketManager : public cocos2d::CCObject {
private:
    ix::WebSocket m_webSocket;
    bool m_isConnected = false;
    std::function<void(std::string const&, std::string const&, std::string const&)> m_onMessageCallback;
    
    std::mutex m_queueMutex;
    struct MessageData {
        std::string sender;
        std::string text;
        std::string avatarUrl;
    };
    std::queue<MessageData> m_messageQueue;

    WebSocketManager();
    ~WebSocketManager() override;

public:
    static WebSocketManager& get();

    void connect(std::string const& url = "wss://echo.websocket.org");
    void disconnect();
    void send(std::string const& text);
    
    void setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback);
    void updateUI();
    void updateDispatch(float dt);
};