#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>

#include "Config.h"
#include "LaundryRoom.h"
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"

namespace telegram {
    // https://core.telegram.org/bots/api#inlinekeyboardmarkup
    struct inlineKeyboardButton {
        String text;
        String callback_data;
    };

    class tg {
    private:
        UniversalTelegramBot* bot = NULL;
        unsigned long lastTimeBotRan = 0;
        void handle_callback(int msg_number, laundry::Room& rm);
        void handle_message(int msg_number, laundry::Room& rm);
        String generate_inline_keyboard(std::vector<inlineKeyboardButton>& buttons);
        String keyboard_claim(laundry::Room& rm);
        String response_claim();
        String response_help();
        String response_start(String name);
        String response_status(laundry::Room& rm);

    public:
        tg(const char* token, WiFiClientSecure& client);
        void check_updates(laundry::Room& rm);
    };
}

#endif  // TELEGRAM_H
