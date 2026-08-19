#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChatCell : public CCNode {
public:
    static ChatCell* create(const std::string& sender, const std::string& message);
    bool init(const std::string& sender, const std::string& message);
};
