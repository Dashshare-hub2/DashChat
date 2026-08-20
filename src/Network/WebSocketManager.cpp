void WebSocketManager::connect() {
    if (m_connected) return;

    std::string baseUrl = Mod::get()->getSettingValue<std::string>("server-url"); // Ví dụ: wss://dashchat-rsuk.onrender.com
    std::string token = Mod::get()->getSettingValue<std::string>("user-token");

    if (baseUrl.empty()) return;

    if (!baseUrl.empty() && baseUrl.back() == '/') {
        baseUrl.pop_back();
    }

    std::string fullUrl = baseUrl;
    if (!token.empty()) {
        fullUrl += "/?token=" + token;
    } else {
        fullUrl += "/";
    }

    m_webSocket.setUrl(fullUrl);

    ix::SocketTLSOptions tlsOptions;
    tlsOptions.tls = true;
    m_webSocket.setTLSOptions(tlsOptions);

    m_webSocket.start();
}