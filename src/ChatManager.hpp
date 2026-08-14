#pragma once

#include <Geode/Geode.hpp>
#include <string>
#include <vector>
#include <memory>

namespace sio {
    class client;
}

struct ChatMessage {
    std::string sender;
    std::string message;
};

class ChatManager {
private:
    std::unique_ptr<sio::client> m_client;
    bool m_connected = false;
    std::vector<ChatMessage> m_messages;

    ChatManager();
    ~ChatManager();

public:
    static ChatManager* get();

    void connect(const std::string& serverUrl);
    void disconnect();
    void sendMessage(const std::string& msg);
    
    const std::vector<ChatMessage>& getMessages() const { return m_messages; }
    bool isConnected() const { return m_connected; }
};