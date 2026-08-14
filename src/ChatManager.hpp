#pragma once

#include <Geode/Geode.hpp>
#include <string>
#include <vector>
#include <memory>

namespace ix {
    class WebSocket;
}

struct ChatMessage {
    std::string sender;
    std::string discordUser;
    std::string message;
};

class ChatManager {
private:
    std::unique_ptr<ix::WebSocket> m_webSocket;
    bool m_connected = false;
    bool m_joined = false;
    std::vector<ChatMessage> m_messages;

    ChatManager();
    ~ChatManager();

public:
    static ChatManager* get();

    void connect();
    void disconnect();
    void joinWithInvite(const std::string& inviteCode);
    void linkDiscord(const std::string& discordUsername, const std::string& discordId);
    void sendMessage(const std::string& msg);
    
    const std::vector<ChatMessage>& getMessages() const { return m_messages; }
    bool isConnected() const { return m_connected; }
    bool isJoined() const { return m_joined; }
};