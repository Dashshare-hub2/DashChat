
#include <sio_client.h>

#ifdef WIN32
    #undef ERROR
    #undef OPTIONAL
    #undef DELETE
    #undef min
    #undef max
    #undef BYTE
#endif

#include "ChatManager.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

ChatManager::ChatManager() {
    m_client = std::make_unique<sio::client>();
}

ChatManager::~ChatManager() {
    if (m_client) {
        m_client->sync_close();
        m_client->clear_con_listeners();
    }
}

ChatManager* ChatManager::get() {
    static ChatManager instance;
    return &instance;
}

void ChatManager::connect(const std::string& serverUrl) {
    if (m_connected) return;

    m_client->set_open_listener([this]() {
        m_connected = true;
        log::info("Connected to DashChat server!");
    });

    m_client->set_close_listener([this](sio::client::close_reason const& reason) {
        m_connected = false;
        log::info("Disconnected from DashChat server.");
    });

    m_client->set_fail_listener([this]() {
        m_connected = false;
        log::error("Failed to connect to DashChat server.");
    });

    m_client->socket()->on("chat_message", sio::socket::event_listener_aux([this](std::string const& name, sio::message::ptr const& data, bool hasAck, sio::message::list &ack_resp) {
        if (data && data->get_flag() == sio::message::flag_object) {
            auto obj = data->get_map();
            if (obj.find("sender") != obj.end() && obj.find("message") != obj.end()) {
                std::string sender = obj["sender"]->get_string();
                std::string message = obj["message"]->get_string();

                Loader::get()->queueInMainThread([this, sender, message]() {
                    m_messages.push_back({sender, message});
                });
            }
        }
    }));

    m_client->connect(serverUrl);
}

void ChatManager::disconnect() {
    if (m_client && m_connected) {
        m_client->close();
        m_connected = false;
    }
}

void ChatManager::sendMessage(const std::string& msg) {
    if (!m_connected || !m_client) return;

    sio::message::ptr pack = sio::object_message::create();
    pack->get_map()["message"] = sio::string_message::create(msg);

    m_client->socket()->emit("send_message", pack);
}