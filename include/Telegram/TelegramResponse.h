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
        String callbackData;
    };

    String responseClaim();
    String responseHelp();
    String responseStart(String name);
    String responseStatus(laundry::Room& rm);
    String responseUnclaim(std::vector<laundry::MachineID>& claimed);
    String generateInlineKeyboard(std::vector<inlineKeyboardButton>& buttons);
    String keyboardClaim(laundry::Room& rm);
    String keyboardUnclaim(std::vector<laundry::MachineID>& claimed);
}

#endif  // TELEGRAM_RESPONSE_H
