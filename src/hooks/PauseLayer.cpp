#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    void customWebBtnCallback(CCObject* sender) {
        geode::utils::web::openURLInBrowser("https://dashchat-rsuk.onrender.com");
    }
};