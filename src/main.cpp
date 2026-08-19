#include <Geode/Geode.hpp>
#include "Network/WebSocketManager.hpp"

using namespace geode::prelude;

$on_mod_loaded {
    log::info("DashChat binary successfully loaded into Geode!");

    new EventListener<EventFilter<ModStateEvent>>(
        +[](ModStateEvent* event) {
            if (event->getState() == ModStateEvent::State::Loaded) {
                log::info("DashChat ModState Loaded event triggered.");
                
                Loader::get()->queueInMainThread([]() {
                    Notification::create("DashChat Loaded!", NotificationIcon::Success)->show();
                    WebSocketManager::get().connect();
                });
            }
            return ListenerResult::Propagate;
        },
        ModStateEventFilter(Mod::get(), ModStateEvent::State::Loaded)
    );
}