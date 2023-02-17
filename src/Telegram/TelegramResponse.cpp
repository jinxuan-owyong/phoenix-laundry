#include "Telegram/TelegramResponse.h"

namespace telegram {
    /**
     * @brief Response for /claim.
     *
     * @return String
     */
    String response_claim() {
        return "Which machine would you like to claim?";
    }

    /**
     * @brief Response for /help.
     *
     * @return String
     */
    String response_help() {
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
    String response_start(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }

    /**
     * @brief Response for /status.
     *
     * @param rm The laundry room instance to reference.
     * @return String
     */
    String response_status(laundry::Room& rm) {
        String output = "*Laundry Room Status*";
        for (auto& machine : rm.machines) {
            output += "\n\u2022 ";
            output += machine.get_name();
            output += ": ";
            output += machine.get_status();
        }
        return output;
    }

    /**
     * @brief Response for /unclaim.
     *
     * @param claimed Machines currently claimed by user.
     * @return String
     */
    String response_unclaim(std::vector<laundry::Machine>& claimed) {
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
    String generate_inline_keyboard(std::vector<inlineKeyboardButton>& buttons) {
        String keyboardJson = "[";
        for (int i = 0; i < buttons.size(); ++i) {
            keyboardJson += "[{\"text\" : ";
            keyboardJson += "\"" + buttons[i].text + "\",";
            keyboardJson += "\"callback_data\" : ";
            keyboardJson += "\"" + buttons[i].callback_data + "\"";
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
    String keyboard_claim(laundry::Room& rm) {
        std::vector<inlineKeyboardButton> buttons;
        for (auto& machine : rm.machines) {
            inlineKeyboardButton b = {
                .text = machine.get_name(),
                .callback_data = "claim-" + String(machine.id)};
            buttons.emplace_back(b);
        }
        return generate_inline_keyboard(buttons);
    }

    /**
     * @brief Generate the inline keyboard for /unclaim.
     *
     * @param claimed Machines currently claimed by user.
     * @return String InlineKeyboardMarkup JSON string.
     */
    String keyboard_unclaim(std::vector<laundry::Machine>& claimed) {
        std::vector<inlineKeyboardButton> buttons;
        for (auto& m : claimed) {
            inlineKeyboardButton b = {
                .text = m.get_name(),
                .callback_data = "unclaim-" + String(m.id)};
            buttons.emplace_back(b);
        }
        return generate_inline_keyboard(buttons);
    }
}
