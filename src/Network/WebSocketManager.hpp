#pragma once
#include <Geode/Geode.hpp>
#include <functional>
#include <string>

class WebSocketManager {
private:
    WebSocketManager() = default;

public:
    static WebSocketManager& get(); 
    
    void setOnMessage(std::function<void(std::string const&, std::string const&, std::string const&)> callback);
    void send(std::string const& msg);
};