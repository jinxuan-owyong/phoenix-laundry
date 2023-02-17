#ifndef TELEGRAM_RESPONSE_H
#define TELEGRAM_RESPONSE_H

#include <vector>

#include "Arduino.h"
#include "LaundryRoom/LaundryMachine.h"
#include "LaundryRoom/LaundryRoom.h"

namespace telegram {
    // https://core.telegram.org/bots/api#inlinekeyboardmarkup
    struct inlineKeyboardButton {
        String text;
        String callback_data;
    };

    String response_claim();
    String response_help();
    String response_start(String name);
    String response_status(laundry::Room& rm);
    String response_unclaim(std::vector<laundry::Machine>& claimed);
    String generate_inline_keyboard(std::vector<inlineKeyboardButton>& buttons);
    String keyboard_claim(laundry::Room& rm);
    String keyboard_unclaim(std::vector<laundry::Machine>& claimed);
}

#endif  // TELEGRAM_RESPONSE_H
