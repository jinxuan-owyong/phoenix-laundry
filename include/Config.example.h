#ifndef CONFIG_H
#define CONFIG_H

namespace config {
    // General
    const int LOOP_DELAY = 200;

    // WiFi
    enum {
        WPA2_PERSONAL,
        WPA2_ENTERPRISE
    };

    // To be set according to WiFi network type
    const int WIFI_CONNECTION_TYPE = WPA2_ENTERPRISE;
    const int WIFI_STATUS_CHECK_DELAY = 500;
    const int WIFI_COUNTER_LIMIT = 60;  // 30 seconds timeout

    // WPA2
    const char* WIFI_SSID = "";
    const char* WIFI_PASSWORD = "";

    // WPA2 Enterprise
    const char* WIFI_SSID = "";
    const char* EAP_IDENTITY = "";
    const char* EAP_USERNAME = "";
    const char* EAP_PASSWORD = "";

    // Telegram
    // token generated from BotFather
    const char* BOT_TOKEN = "";
}

#endif  // CONFIG_H
