#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#include <vector>

#include "LaundryConfig.h"

namespace config {
    enum {
        WPA2_PERSONAL,
        WPA2_ENTERPRISE
    };

    class cfg {
    public:
        // General
        const int LOOP_DELAY = 200;
        const int PING_INTERVAL = 10 * 60 * 1000;  // 10 minutes
        const char* PING_TARGET_IP = "8.8.8.8";

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
        const String COMMAND_UNCLAIM = "/unclaim";
        const String MARKDOWN = "Markdown";
        const String CALLBACK_QUERY = "callback_query";

        // Laundry Room
        std::vector<int> PHOENIX_LAUNDRY_ROOM = {
            laundry::ID_DRYER_A,
            laundry::ID_DRYER_B,
            laundry::ID_WASHER_A,
            laundry::ID_WASHER_B};

        const int TIME_BETWEEN_SCANS = 1 * 60 * 1000;  // 1 minute interval
    };
}

#endif  // CONFIG_H
