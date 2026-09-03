#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class ChatCell : public cocos2d::CCNode {
public:
    static ChatCell* create();
    bool init();

    geode::EventListener<geode::utils::web::WebTask> m_avatarTaskListener;
};