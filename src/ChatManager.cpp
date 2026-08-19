#include "ChatManager.hpp"

ChatManager* ChatManager::instance = nullptr;

ChatManager* ChatManager::get() {
    if (!instance) {
        instance = new ChatManager();
    }
    return instance;
}

void ChatManager::connect() {
    if (m_connected) return;
    m_connected = true;
    
    m_messages.clear();
    addMessage("System", "Welcome to DashChat!");
    
    geode::log::info("DashChat Connected!");
}

void ChatManager::disconnect() {
    m_connected = false;
    geode::log::info("DashChat Disconnected!");
}

bool ChatManager::isConnected() const {
    return m_connected;
}