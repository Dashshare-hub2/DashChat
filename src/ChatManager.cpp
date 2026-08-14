#pragma once
#include <sio_client.h>
#ifdef WIN32
    #undef OPTIONAL
    #undef DELETE
    #undef min
    #undef max
#endif
#include <Geode/Geode.hpp>
#include <string>
#include <functional>

using namespace geode::prelude;

struct ChatMessage {
    std::string discordName;
    std::string gdUser;
    std::string avatarUrl;
    std::string message;
    std::string room;
};

class ChatManager {
private:
    sio::client m_client;
    bool m_connected = false;
    std::string m_currentRoom = "global";
    std::string m_discordName = "";
    std::string m_avatarUrl = "";

    std::function<void(const ChatMessage&)> m_onMessageCallback;
    std::function<void(const std::string&, const std::string&, const std::string&)> m_onInviteCallback;

public:
    static ChatManager* get() {
        static ChatManager instance;
        return &instance;
    }

    void connect(const std::string& serverUrl) {
        m_client.set_open_listener([this]() {
            m_connected = true;
            log::info("Connected to DashChat Server!");
        });

        m_client.socket()->on("receive-message", sio::sio_message::list_cb([this](const sio::event& ev) {
            auto data = ev.get_message()->get_map();
            ChatMessage msg;
            msg.discordName = data["discordName"]->get_string();
            msg.gdUser = data["gdUser"]->get_string();
            msg.avatarUrl = data["avatarUrl"]->get_string();
            msg.message = data["message"]->get_string();
            msg.room = data["room"]->get_string();

            Loader::get()->queueInMainThread([this, msg]() {
                if (m_onMessageCallback) m_onMessageCallback(msg);
            });
        }));

        m_client.socket()->on("player-invited", sio::sio_message::list_cb([this](const sio::event& ev) {
            auto data = ev.get_message()->get_map();
            std::string inviterGD = data["inviterGD"]->get_string();
            std::string targetGD = data["targetGD"]->get_string();
            std::string roomCode = data["roomCode"]->get_string();

            Loader::get()->queueInMainThread([this, inviterGD, targetGD, roomCode]() {
                if (m_onInviteCallback) m_onInviteCallback(inviterGD, targetGD, roomCode);
            });
        }));

        m_client.connect(serverUrl);
    }

    void authenticateToken(const std::string& token, const std::string& gdUser) {
        sio::message::ptr data = sio::object_message::create();
        data->get_map()["authToken"] = sio::string_message::create(token);
        data->get_map()["gdUser"] = sio::string_message::create(gdUser);
        m_client.socket()->emit("authenticate", data);
    }

    void joinRoom(const std::string& roomCode) {
        m_client.socket()->emit("join-room", roomCode);
        m_currentRoom = roomCode;
    }

    void joinGlobal() {
        m_client.socket()->emit("join-global");
        m_currentRoom = "global";
    }

    void sendMessage(const std::string& text) {
        m_client.socket()->emit("send-message", text);
    }

    void sendInvite(const std::string& targetGD, const std::string& roomCode) {
        sio::message::ptr data = sio::object_message::create();
        data->get_map()["targetGD"] = sio::string_message::create(targetGD);
        data->get_map()["roomCode"] = sio::string_message::create(roomCode);
        m_client.socket()->emit("invite-player", data);
    }

    void setOnMessageCallback(std::function<void(const ChatMessage&)> cb) { m_onMessageCallback = cb; }
    void setOnInviteCallback(std::function<void(const std::string&, const std::string&, const std::string&)> cb) { m_onInviteCallback = cb; }
    std::string getCurrentRoom() const { return m_currentRoom; }
};