#include "CreateRoomPopup.hpp"

using namespace geode::prelude;

CreateRoomPopup* CreateRoomPopup::create() {
    auto ret = new CreateRoomPopup();
    if (ret && ret->initAnchored(320.0f, 210.0f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CreateRoomPopup::setup() {
    this->setTitle("Create Chat Room");

    auto winSize = m_mainLayer->getContentSize();

    // Room Name Input
    m_roomNameInput = TextInput::create(240.0f, "Room Name...", "chatFont.fnt");
    m_roomNameInput->setPosition({ winSize.width / 2.0f, winSize.height - 65.0f });
    m_roomNameInput->setScale(0.85f);
    m_mainLayer->addChild(m_roomNameInput);

    // Room Password Input
    m_passwordInput = TextInput::create(240.0f, "Password (Optional)", "chatFont.fnt");
    m_passwordInput->setPosition({ winSize.width / 2.0f, winSize.height - 115.0f });
    m_passwordInput->setScale(0.85f);
    m_mainLayer->addChild(m_passwordInput);

    // Create Button
    auto menu = CCMenu::create();
    menu->setPosition({ winSize.width / 2.0f, 30.0f });

    auto createBtnSprite = ButtonSprite::create("Create", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto createBtn = CCMenuItemSpriteExtra::create(
        createBtnSprite,
        this,
        menu_selector(CreateRoomPopup::onCreateRoom)
    );

    menu->addChild(createBtn);
    m_mainLayer->addChild(menu);

    return true;
}

void CreateRoomPopup::onCreateRoom(CCObject* sender) {
    std::string roomName = m_roomNameInput->getString();
    std::string pass = m_passwordInput->getString();

    if (roomName.empty()) {
        FLAlertLayer::create("Error", "Please enter a valid room name!", "OK")->show();
        return;
    }

    std::string fbUrl = Mod::get()->getSavedValue<std::string>("firebase_db_url", "https://dashchat-server-default-rtdb.firebaseio.com");
    std::string reqUrl = fbUrl + "/rooms/" + roomName + ".json";

    mat_json::Value payload = mat_json::Object {
        { "created_at", static_cast<double>(time(nullptr)) },
        { "password", pass }
    };

    m_createRoomTask.bind([this, roomName](web::WebTask::Event* event) {
        if (auto res = event->getValue()) {
            if (res->ok()) {
                FLAlertLayer::create("Success", "Room created successfully: " + roomName, "OK")->show();
                this->onClose(nullptr);
            } else {
                FLAlertLayer::create("Error", "Failed to create room on Firebase DB!", "OK")->show();
            }
        }
    });

    auto req = web::WebRequest();
    req.bodyString(payload.dump());
    m_createRoomTask.setFilter(req.put(reqUrl));
}