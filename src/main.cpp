#include <Arduino.h>

#include "Config.h"
#include "Telegram.h"

WiFiClientSecure secured_client;
UniversalTelegramBot tgBot(config::BOT_TOKEN, secured_client);
unsigned long lastTimeBotRan;

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to connect to ");
        Serial.println(config::WIFI_SSID);
        WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);
        delay(5000);
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
}

void loop() {
    if (millis() > lastTimeBotRan + telegram::BOT_MTBS) {
        telegram::check_updates(tgBot);
        lastTimeBotRan = millis();
    }
    delay(config::LOOP_DELAY);
}
