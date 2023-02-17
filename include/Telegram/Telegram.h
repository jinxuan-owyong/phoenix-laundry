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
        void handle_callback(int msg_number, laundry::Room& rm);
        void handle_message(int msg_number, laundry::Room& rm);
        static bool is_command(const String& msg, const String& cmd);

    public:
        tg(const char* token, WiFiClientSecure& client);
        unsigned long lastTimeBotRan = 0;
        void check_updates(laundry::Room& rm);
        void send_message(String msg, String target, String keyboard = "");
    };
}

#endif  // TELEGRAM_H
