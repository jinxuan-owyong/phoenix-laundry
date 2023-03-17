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
    String responseStats();
    String responseStatus(laundry::Room& rm);
    String responseUnclaim(std::vector<laundry::MachineID>& claimed);
    String generateInlineKeyboard(std::vector<inlineKeyboardButton>& buttons);
    String keyboardClaim(laundry::Room& rm);
    String keyboardUnclaim(std::vector<laundry::MachineID>& claimed);
    String keyboardConfirm(String callbackId);

    static const String RESPONSE_UNAUTHORISED = "You are not authorised to perform this action.";
    static const String RESPONSE_CONFIRM_RESTART = "Are you sure you want to restart the bot?";
    static const String RESPONSE_RESTARTING = "Restarting bot.";
}

#endif  // TELEGRAM_RESPONSE_H
