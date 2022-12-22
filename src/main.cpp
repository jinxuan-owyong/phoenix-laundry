#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "ESP32Ping.h"
#include "LaundryRoom.h"
#include "Telegram.h"
#include "WiFiClientSecure.h"

config::cfg constants;
WiFiClientSecure secured_client;
telegram::tg tgBot(constants.BOT_TOKEN, secured_client);
unsigned long lastTimeBotRan = 0, lastPingTime = 0;
laundry::Room phoenix(constants.PHOENIX_LAUNDRY_ROOM);

void setup() {
    Serial.begin(115200);

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
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
}

void loop() {
    // WiFi keep-alive service
    if (millis() > lastPingTime + constants.PING_INTERVAL) {
        bool success = Ping.ping(constants.PING_TARGET_IP);
        if (!success) {
            ESP.restart();
        }
        lastPingTime = millis();
    }

    // telegram bot process
    if (millis() > lastTimeBotRan + constants.BOT_MTBS) {
        tgBot.check_updates(phoenix);
    }

    delay(constants.LOOP_DELAY);
}
