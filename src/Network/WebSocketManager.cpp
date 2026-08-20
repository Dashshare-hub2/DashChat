void WebSocketManager::send(std::string const& text) {
    if (!m_connected || text.empty()) return;

    matjson::Value json;
    json["text"] = text;

    m_webSocket.send(json.dump());
}