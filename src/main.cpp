#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "ESP32Ping.h"
#include "LaundryRoom.h"
#include "Telegram.h"
#include "WiFiClientSecure.h"

// general
config::cfg constants;
void setup_wifi();

// telegram bot
WiFiClientSecure secured_client;
telegram::tg tgBot(constants.BOT_TOKEN, secured_client);
unsigned long lastPingTime = 0;

// laundry room
laundry::config_t room_config(constants.INPUT_DRYER_A, constants.INPUT_DRYER_B, -1, -1);
laundry::Room phoenix(constants.PHOENIX_LAUNDRY_ROOM, room_config);
std::unordered_map<int, int> prev_status;
std::unordered_map<int, int> prev_update;
std::vector<int> GPIO_INPUT = {laundry::ID_DRYER_A, laundry::ID_DRYER_B};

void setup() {
    Serial.begin(115200);
    setup_wifi();
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
    for (auto& input : GPIO_INPUT) {                      // trigger status update on startup
        prev_status[input] = laundry::ID_READY;
        prev_update[input] = -1 * constants.TIME_BETWEEN_SCANS;
    }
}

void loop() {
    // WiFi keep-alive service
    if (millis() > lastPingTime + constants.PING_INTERVAL) {
        bool success = Ping.ping(constants.PING_TARGET_IP);
        if (!success) {
            Serial.println("Ping failed.");
            ESP.restart();
        }
        lastPingTime = millis();
    }

    // scan for updated machine status
    String message = "";
    for (auto& input : GPIO_INPUT) {
        if (millis() > prev_update[input] + constants.TIME_BETWEEN_SCANS) {
            String debug = "\n";
            int status = phoenix.get_best_result(constants.NUM_READINGS,
                                                 constants.BEST_THRESHOLD,
                                                 input,
                                                 &debug);
            if (status != prev_status[input]) {
                phoenix.set_machine_status(input, status);
                prev_status[input] = status;
                message += "Status updated (";
                message += phoenix.get_machine_name(input) + "): ";
                message += phoenix.get_machine_status(input) + "\n";
                message += debug + (debug != "" ? "\n" : "");
            }
            prev_update[input] = millis();
        }
    }

    // Send debug message
    if (message != "" && constants.DEPLOY_ENV) {
        tgBot.send_message(message, constants.GROUP_ID_DEV);
    }

    // telegram bot process
    if (millis() > tgBot.lastTimeBotRan + constants.BOT_MTBS) {
        tgBot.check_updates(phoenix);
    }

    delay(constants.LOOP_DELAY);
}

void setup_wifi() {
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200);

    // Set WiFi credentials according to connection type
    switch (constants.WIFI_CONNECTION_TYPE) {
    case config::WPA2_PERSONAL:
        WiFi.begin(constants.WIFI_SSID, constants.WIFI_PASSWORD);
        break;
    case config::WPA2_ENTERPRISE:
        WiFi.begin(constants.WIFI_SSID,
                   WPA2_AUTH_PEAP,
                   constants.EAP_IDENTITY,
                   constants.EAP_USERNAME,
                   constants.EAP_PASSWORD);
        break;
    }

    Serial.print("Attempting to connect to ");
    Serial.print(constants.WIFI_SSID);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(constants.WIFI_STATUS_CHECK_DELAY);
        Serial.print('.');
        ++counter;
        // re-attempt connection
        if (counter >= constants.WIFI_COUNTER_LIMIT) {
            ESP.restart();
        }
    }

    Serial.println("\nConnected to WiFi");
    Serial.println(WiFi.localIP());
}
