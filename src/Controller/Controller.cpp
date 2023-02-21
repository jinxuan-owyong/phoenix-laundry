#include "Controller/Controller.h"

void Controller::setup() {
    lastPingTime = 0;
    tgBot = telegram::tg(config::BOT_TOKEN, securedClient);
    securedClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
    room = laundry::Room(config::PHOENIX_LAUNDRY_ROOM, roomConfig);
    tgBot.sendMessage("Bot started.", config::GROUP_ID_DEV);
    room.refreshMachines(NULL);
}

void Controller::start() {
    while (true) {
        checkWiFiStatus();
        updateRoomStatus();
        updateUsers();
        checkTelegram();
        delay(config::LOOP_DELAY);
    }
}

void Controller::checkWiFiStatus() {
    // WiFi keep-alive service
    if (millis() > lastPingTime + config::PING_INTERVAL) {
        bool isOnline = false;
        for (int i = 0; i < config::PING_RETRY_LIMIT && !isOnline; ++i) {
            IPAddress ip;
            int dnsSuccess = WiFi.hostByName((String(i) + config::PING_TARGET_ADDRESS).c_str(), ip);  // check for DNS failure
            bool pingSuccess = Ping.ping(ip);                                                         // check connection
            Serial.println(ip.toString());
            Serial.println(dnsSuccess);
            Serial.println(pingSuccess);

            isOnline = (dnsSuccess == 1) && pingSuccess;
            lastPingTime = millis();
            delay(config::LOOP_DELAY);
        }
        if (!isOnline) {
            Serial.println("Ping failed.");
            tgBot.sendMessage("WiFi check failed.", config::GROUP_ID_DEV);
            ESP.restart();
        }
    }
}

void Controller::updateRoomStatus() {
    // scan for updated machine status
    String message = "";
    if (millis() > room.getLastUpdated() + config::TIME_BETWEEN_SCANS) {
        for (auto& gpioPin : config::PHOENIX_LAUNDRY_ROOM) {
            String debug = "\n";
            room.refreshMachines(&debug);
            message += debug + (debug != "" ? "\n" : "");
        }
    }
    sendDebugMessage(message);
}

void Controller::sendDebugMessage(String message) {
    // Send debug message
    if (!message.isEmpty() && config::DEPLOY_ENV == config::DEVELOPMENT) {
        tgBot.sendMessage(message, config::GROUP_ID_DEV);
    }
}

void Controller::updateUsers() {
    // update users on cycle completion
    auto completedMachines = room.getCompletedMachines();
    for (auto& m : completedMachines) {
        String msg = "✅ " + m.getName() + " done!";
        String userId = m.getCurrUser().getTelegramId();

        if (!userId.isEmpty()) {  // send message to user
            tgBot.sendMessage(msg, userId);
        } else {  // send message to group
            tgBot.sendMessage(msg, (config::DEPLOY_ENV == config::PRODUCTION)
                                       ? config::GROUP_ID_PROD
                                       : config::GROUP_ID_DEV);
        }
        delay(config::MESSAGE_INTERVAL);
    }
}

void Controller::checkTelegram() {
    // telegram bot process
    if (millis() > tgBot.lastTimeBotRan + config::BOT_MTBS) {
        tgBot.checkUpdates(room);
    }
}