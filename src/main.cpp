#include <Arduino.h>
#include <WiFi.h>

#include "Config/Config.h"
#include "ESP32Ping.h"
#include "LaundryRoom/LaundryRoom.h"
#include "Telegram/Telegram.h"
#include "WiFiClientSecure.h"

// general
void setup_wifi();

// telegram bot
WiFiClientSecure secured_client;
telegram::tg tgBot(config::BOT_TOKEN, secured_client);
unsigned long lastPingTime = 0;

// laundry room
laundry::config_t room_config(config::INPUT_DRYER_A, config::INPUT_DRYER_B, -1, -1);
laundry::Room phoenix(config::PHOENIX_LAUNDRY_ROOM, room_config);
std::vector<int> GPIO_INPUT = {laundry::ID_DRYER_A, laundry::ID_DRYER_B};

void setup() {
    Serial.begin(115200);
    setup_wifi();
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
    if (config::DEPLOY_ENV == config::DEVELOPMENT) {
        tgBot.send_message("Bot started.", config::GROUP_ID_DEV);
    }
    phoenix.refreshMachines(NULL);
}

void loop() {
    // WiFi keep-alive service
    if (millis() > lastPingTime + config::PING_INTERVAL) {
        bool is_online = false;
        for (int i = 0; i < config::PING_RETRY_LIMIT && !is_online; ++i) {
            IPAddress ip;
            int dns_success = WiFi.hostByName((String(i) + config::PING_TARGET_ADDRESS).c_str(), ip);  // check for DNS failure
            bool ping_success = Ping.ping(ip);                                                         // check connection
            Serial.println(ip.toString());
            Serial.println(dns_success);
            Serial.println(ping_success);

            is_online = (dns_success == 1) && ping_success;
            lastPingTime = millis();
            delay(config::LOOP_DELAY);
        }
        if (!is_online) {
            Serial.println("Ping failed.");
            ESP.restart();
        }
    }

    // scan for updated machine status
    String message = "";
    if (millis() > phoenix.getLastUpdated() + config::TIME_BETWEEN_SCANS) {
        for (auto& gpioPin : config::PHOENIX_LAUNDRY_ROOM) {
            String debug = "\n";
            phoenix.refreshMachines(&debug);
            message += debug + (debug != "" ? "\n" : "");
        }
    }

    // Send debug message
    if (message != "" && config::DEPLOY_ENV) {
        tgBot.send_message(message, config::GROUP_ID_DEV);
    }

    // update users on cycle completion
    auto completed_machines = phoenix.getCompletedMachines();
    for (auto& m : completed_machines) {
        String msg = "✅ " + m.getName() + " done!";
        String userId = m.getCurrUser().getTelegramId();

        if (!userId.isEmpty()) {  // send message to user
            tgBot.send_message(msg, userId);
        } else {  // send message to group
            tgBot.send_message(msg, (config::DEPLOY_ENV == config::PRODUCTION)
                                        ? config::GROUP_ID_PROD
                                        : config::GROUP_ID_DEV);
        }
        delay(config::MESSAGE_INTERVAL);
    }

    // telegram bot process
    if (millis() > tgBot.lastTimeBotRan + config::BOT_MTBS) {
        tgBot.check_updates(phoenix);
    }

    delay(config::LOOP_DELAY);
}

void setup_wifi() {
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200);

    // Set WiFi credentials according to connection type
    switch (config::WIFI_CONNECTION_TYPE) {
    case config::WPA2_PERSONAL:
        WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);
        break;
    case config::WPA2_ENTERPRISE:
        WiFi.begin(config::WIFI_SSID,
                   WPA2_AUTH_PEAP,
                   config::EAP_IDENTITY,
                   config::EAP_USERNAME,
                   config::EAP_PASSWORD);
        break;
    }

    Serial.print("Attempting to connect to ");
    Serial.print(config::WIFI_SSID);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(config::WIFI_STATUS_CHECK_DELAY);
        Serial.print('.');
        ++counter;
        // re-attempt connection
        if (counter >= config::WIFI_COUNTER_LIMIT) {
            ESP.restart();
        }
    }

    Serial.println("\nConnected to WiFi");
    Serial.println(WiFi.localIP());
}
