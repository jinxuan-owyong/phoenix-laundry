#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

#include "LaundryRoom.h"

namespace config {
    enum {
        WPA2_PERSONAL,
        WPA2_ENTERPRISE
    };

    class cfg {
    public:
        // General
        const int LOOP_DELAY = 200;

        // WiFi
        // To be set according to WiFi network type
        const int WIFI_CONNECTION_TYPE = WPA2_PERSONAL;
        const int WIFI_STATUS_CHECK_DELAY = 500;
        const int WIFI_COUNTER_LIMIT = 60;  // 30 seconds timeout

        // WiFi Credentials
        const char* WIFI_SSID = "";
        const char* WIFI_PASSWORD = "";  // leave empty for WPA2 Enterprise
        const char* EAP_IDENTITY = "";   // leave EAP fields empty for WPA2 Personal
        const char* EAP_USERNAME = "";
        const char* EAP_PASSWORD = "";

        // Telegram
        // token generated from BotFather
        const char* BOT_TOKEN = "";
        const unsigned long BOT_MTBS = 1000;  // mean time between scan messages

        const String COMMAND_CLAIM = "/claim";
        const String COMMAND_HELP = "/help";
        const String COMMAND_START = "/start";
        const String COMMAND_STATUS = "/status";
        const String MARKDOWN = "Markdown";
        const String CALLBACK_QUERY = "callback_query";

        // Laundry Room
        std::vector<int> PHOENIX_LAUNDRY_ROOM = {
            laundry::ID_DRYER_A,
            laundry::ID_DRYER_B,
            laundry::ID_WASHER_A,
            laundry::ID_WASHER_B};
    };
}

#endif  // CONFIG_H
