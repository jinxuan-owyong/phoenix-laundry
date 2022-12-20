#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "LaundryRoom.h"
#include "Telegram.h"
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"

WiFiClientSecure secured_client;
UniversalTelegramBot tgBot(config::BOT_TOKEN, secured_client);
unsigned long lastTimeBotRan;
laundry::Room phoenix(config::PHOENIX_LAUNDRY_ROOM);

void setup() {
    Serial.begin(115200);

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
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
}

void loop() {
    if (millis() <= lastTimeBotRan + telegram::BOT_MTBS) {
        delay(config::LOOP_DELAY);
        return;
    }

    // telegram bot process
    int numNewMessages = tgBot.getUpdates(tgBot.last_message_received + 1);
    while (numNewMessages) {
        Serial.print("new message: ");
        Serial.println(String(numNewMessages));

        for (int i = 0; i < numNewMessages; i++) {
            // Chat id of the requester
            String chat_id = String(tgBot.messages[i].chat_id);

            // Print the received message
            String text = tgBot.messages[i].text;
            Serial.println(text);

            if (text == telegram::COMMAND_START) {
                String from_name = tgBot.messages[i].from_name;
                tgBot.sendMessage(chat_id, telegram::response_start(from_name), "");
            } else if (text == telegram::COMMAND_HELP) {
                tgBot.sendMessage(chat_id, telegram::response_help(), telegram::MARKDOWN);
            }
        }

        numNewMessages = tgBot.getUpdates(tgBot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
}
