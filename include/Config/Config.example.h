#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#include <vector>

#include "LaundryConfig.h"

namespace config {
    enum {
        WPA2_PERSONAL,
        WPA2_ENTERPRISE,
        PRODUCTION,
        DEVELOPMENT
    };

    // General
    static const int LOOP_DELAY = 200;
    static const int PING_INTERVAL = 3 * 60 * 1000;             // 3 minutes
    static const String PING_TARGET_ADDRESS = ".pool.ntp.org";  // add prefix 0, 1, 2, 3
    static const int PING_RETRY_LIMIT = 4;                      // max prefix 3 for ping address
    static const int DEPLOY_ENV = DEVELOPMENT;

    // WiFi
    // To be set according to WiFi network type
    static const int WIFI_CONNECTION_TYPE = WPA2_ENTERPRISE;
    static const int WIFI_STATUS_CHECK_DELAY = 500;
    static const int WIFI_COUNTER_LIMIT = 60;  // 30 seconds timeout

    // WiFi Credentials
    const char* WIFI_SSID = "";
    const char* WIFI_PASSWORD = "";  // leave empty for WPA2 Enterprise
    const char* EAP_IDENTITY = "";   // leave EAP fields empty for WPA2 Personal
    const char* EAP_USERNAME = "";
    const char* EAP_PASSWORD = "";

    // Telegram
    // token generated from BotFather
    static const char* BOT_TOKEN = "";
    static const unsigned long BOT_MTBS = 1000;  // mean time between scan messages
    static const int MESSAGE_INTERVAL = 200;
    static const String GROUP_ID_DEV = "";
    static const String GROUP_ID_PROD = "";

    static const String COMMAND_CLAIM = "/claim";
    static const String COMMAND_HELP = "/help";
    static const String COMMAND_START = "/start";
    static const String COMMAND_STATUS = "/status";
    static const String COMMAND_UNCLAIM = "/unclaim";
    static const String MARKDOWN = "Markdown";
    static const String CALLBACK_QUERY = "callback_query";

    // Laundry Room
    static std::vector<int> PHOENIX_LAUNDRY_ROOM = {
        laundry::ID_DRYER_A,
        laundry::ID_DRYER_B,
        laundry::ID_WASHER_A,
        laundry::ID_WASHER_B};

    static const int TIME_BETWEEN_SCANS = 1 * 60 * 1000;  // 1 minute interval
    static const int NUM_READINGS = 3;
    static const int BEST_THRESHOLD = 2;

    // Sensor input
    // GPIO Pins (ADC 1 only)
    static const int INPUT_DRYER_A = 32;
    static const int INPUT_DRYER_B = 33;
    static const int INPUT_WASHER_A = 34;
    static const int INPUT_WASHER_B = 35;
}

#endif  // CONFIG_H
