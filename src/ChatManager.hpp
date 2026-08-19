#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include <string>

struct ChatMessage {
    std::string author;
    std::string content;
};

class ChatManager {
private:
    static ChatManager* instance;
    bool m_connected = false;
    std::vector<ChatMessage> m_messages;

public:
    static ChatManager* get();
    void connect();
    void disconnect();
    bool isConnected() const;
    
    const std::vector<ChatMessage>& getMessages() const { return m_messages; }
    void addMessage(const std::string& author, const std::string& content) {
        m_messages.push_back({author, content});
    }
};