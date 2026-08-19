#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include "Network/WebSocketManager.hpp"

using namespace geode::prelude;

$on_mod_loaded {
    log::info("DashChat binary loaded successfully!");

    Loader::get()->queueInMainThread([]() {
        Notification::create("DashChat v1.0.0 Ready!", NotificationIcon::Success)->show();
        WebSocketManager::get().connect();
    });
}