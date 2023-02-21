#include "Telegram/TelegramResponse.h"

namespace telegram {
    /**
     * @brief Response for /claim.
     *
     * @return String
     */
    String responseClaim() {
        return "Which machine would you like to claim?";
    }

    /**
     * @brief Response for /help.
     *
     * @return String
     */
    String responseHelp() {
        String output = "*Laundry Bot Help Menu*\n";
        output += "Commands:\n";
        output += "/claim - Tag a dryer/washer to your telegram handle\n";
        output += "/help - Show this message\n";
        output += "/start - Initialise bot\n";
        output += "/status - Get the status of the laundry room\n";
        output += "/unclaim - Untag a dryer/washer that you accidentally claimed";
        return output;
    }

    /**
     * @brief Response for /start.
     *
     * @param name User's name.
     * @return String
     */
    String responseStart(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }

    /**
     * @brief Response for /status.
     *
     * @param rm The laundry room instance to reference.
     * @return String
     */
    String responseStatus(laundry::Room& rm) {
        return rm.getRoomStatus();
    }

    /**
     * @brief Response for /unclaim.
     *
     * @param claimed Machines currently claimed by user.
     * @return String
     */
    String responseUnclaim(std::vector<laundry::MachineID>& claimed) {
        if (claimed.size() == 0) {
            return "You have not claimed any machines.";
        }

        return "Which machine would you like to unclaim?";
    }

    /**
     * @brief Generate an inline keyboard JSON string.
     *
     * @param buttons The inline keyboard buttons.
     * @return String InlineKeyboardMarkup JSON string.
     */
    String generateInlineKeyboard(std::vector<inlineKeyboardButton>& buttons) {
        String keyboardJson = "[";
        for (int i = 0; i < buttons.size(); ++i) {
            keyboardJson += "[{\"text\" : ";
            keyboardJson += "\"" + buttons[i].text + "\",";
            keyboardJson += "\"callback_data\" : ";
            keyboardJson += "\"" + buttons[i].callbackData + "\"";
            keyboardJson += "}]";

            if (i != buttons.size() - 1) {
                keyboardJson += ",";
            }
        }
        keyboardJson += "]";
        return keyboardJson;
    }

    /**
     * @brief Generate the inline keyboard for /claim.
     *
     * @param rm The laundry room instance to reference.
     * @return String InlineKeyboardMarkup JSON string.
     */
    String keyboardClaim(laundry::Room& rm) {
        std::vector<inlineKeyboardButton> buttons;
        auto ids = rm.getMachineIds();
        for (auto& id : ids) {
            inlineKeyboardButton b = {
                .text = laundry::Machine::getNameById(id),
                .callbackData = "claim-" + String(id)};
            buttons.emplace_back(b);
        }
        return generateInlineKeyboard(buttons);
    }

    /**
     * @brief Generate the inline keyboard for /unclaim.
     *
     * @param claimed Machines currently claimed by user.
     * @return String InlineKeyboardMarkup JSON string.
     */
    String keyboardUnclaim(std::vector<laundry::MachineID>& claimed) {
        std::vector<inlineKeyboardButton> buttons;
        for (auto& id : claimed) {
            inlineKeyboardButton b = {
                .text = laundry::Machine::getNameById(id),
                .callbackData = "unclaim-" + String(id)};
            buttons.emplace_back(b);
        }
        return generateInlineKeyboard(buttons);
    }
}
