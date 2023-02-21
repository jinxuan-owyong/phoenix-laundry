#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Config/Config.h"
#include "ESP32Ping.h"
#include "LaundryRoom/LaundryRoom.h"
#include "Telegram/Telegram.h"
#include "WiFiClientSecure.h"

class Controller {
private:
    // telegram bot
    WiFiClientSecure securedClient;
    telegram::tg tgBot;
    unsigned long lastPingTime;

    // laundry room
    laundry::config_t roomConfig = laundry::config_t(config::INPUT_DRYER_A,
                                                     config::INPUT_DRYER_B,
                                                     config::INPUT_WASHER_A,
                                                     config::INPUT_WASHER_B);
    laundry::Room room;
    std::vector<int> GPIO_INPUT = {laundry::ID_DRYER_A,
                                   laundry::ID_DRYER_B,
                                   laundry::ID_WASHER_A,
                                   laundry::ID_WASHER_B};

    void checkWiFiStatus();
    void updateRoomStatus();
    void sendDebugMessage(String message);
    void updateUsers();
    void checkTelegram();

public:
    void setup();
    void start();
};

#endif  // CONTROLLER_H