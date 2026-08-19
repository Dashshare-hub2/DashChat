#include <Geode/Geode.hpp>
#include "Network/WebSocketManager.hpp"

using namespace geode::prelude;

$on_mod_loaded {
    log::info("DashChat entry point triggered!");
    
    geode::queueInMainThread([]() {
        WebSocketManager::get().connect();
    });
}