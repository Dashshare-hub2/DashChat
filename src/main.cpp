#include <Geode/Geode.hpp>
#include "Network/WebSocketManager.hpp"

using namespace geode::prelude;

$on_mod_loaded {
    log::info("==========================================");
    log::info("   DASHCHAT ENTRY POINT EXECUTED!       ");
    log::info("==========================================");

    geode::queueInMainThread([]() {
        Notification::create("DashChat v1.0.0 Loaded!", NotificationIcon::Success)->show();
        WebSocketManager::get().connect();
    });
}