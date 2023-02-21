#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>

#include <vector>

#include "Config/Config.h"
#include "LaundryRoom/LaundryRoom.h"
#include "TelegramResponse.h"
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"

namespace telegram {
    class tg {
    private:
        UniversalTelegramBot* bot = NULL;
        void handleCallback(int msg_number, laundry::Room& rm);
        void handleMessage(int msg_number, laundry::Room& rm);
        static bool isCommand(const String& msg, const String& cmd);

    public:
        tg() {}
        tg(const char* token, WiFiClientSecure& client);
        unsigned long lastTimeBotRan = 0;
        void checkUpdates(laundry::Room& rm);
        void sendMessage(String msg, String target, String keyboard = "");
    };
}

#endif  // TELEGRAM_H
