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
     * @brief Get heap usage statistics
     *
     * @return String
     */
    String responseStats() {
        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t minFreeHeap = ESP.getMinFreeHeap();
        uint32_t totalHeap = ESP.getHeapSize();
        String result = "Current heap available: " + String(freeHeap) + " " + String(freeHeap / totalHeap * 100);
        result += "% used\nMin free heap: " + String(minFreeHeap) + " " + String((totalHeap - minFreeHeap) / totalHeap * 100);
        result += "% used";
        return result;
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

    /**
     * @brief Generate inline keyboard with yes and no options.
     * Adds callbackId to the front of callback data of button.
     *
     * @param callbackId Prefix to be checked in callback data.
     * @return String inline keyboard buttons with callback data
     * <callbackId>-yes or <callbackId>-no
     */
    String keyboardConfirm(String callbackId) {
        inlineKeyboardButton btnYes = {
            .text = "Yes",
            .callbackData = callbackId + "-yes"};
        inlineKeyboardButton btnNo = {
            .text = "No",
            .callbackData = callbackId + "-no"};
        std::vector<inlineKeyboardButton> btns = {btnYes, btnNo};
        return generateInlineKeyboard(btns);
    }
}
