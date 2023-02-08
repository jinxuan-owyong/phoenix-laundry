#include "Telegram.h"

namespace telegram {
    /**
     * @brief Construct a new tg::tg object.
     *
     * @param token Bot API token from @BotFather.
     * @param client WiFiClientSecure instance.
     */
    tg::tg(const char* token, WiFiClientSecure& client) {
        bot = new UniversalTelegramBot(token, client);
    }

    /**
     * @brief Main method to be called in bot instance. Checks for new messages
     * and responds to commands and callbacks accordingly.
     *
     * @param rm The laundry room instance to reference.
     */
    void tg::check_updates(laundry::Room& rm) {
        int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
        while (numNewMessages) {
            Serial.print("new message: ");
            Serial.println(String(numNewMessages));

            for (int i = 0; i < numNewMessages; i++) {
                auto& curr_msg = bot->messages[i];
                // Chat id of the requester
                String chat_id = String(curr_msg.chat_id);

                // Print the received message
                String text = curr_msg.text;
                Serial.println(text);

                if (curr_msg.type == config::CALLBACK_QUERY) {
                    handle_callback(i, rm);
                } else {
                    handle_message(i, rm);
                }
            }
            numNewMessages = bot->getUpdates(bot->last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }

    /**
     * @brief Handle callback queries, used for claiming and unclaiming machines.
     *
     * @param msg_number Message number as determined by UniversalTelegramBot::getUpdates().
     * @param rm The laundry room instance to reference.
     */
    void tg::handle_callback(int msg_number, laundry::Room& rm) {
        auto& curr_msg = bot->messages[msg_number];
        String text = curr_msg.text;
        laundry::User curr_user(curr_msg.from_name, curr_msg.from_id, curr_msg.username);
        if (text.substring(0, 5) == "claim") {  // claiming machine
            int claim_id = (text.substring(text.indexOf('-') + 1, text.length())).toInt();
            String res = rm.claim(claim_id, curr_user);
            bot->answerCallbackQuery(curr_msg.query_id,
                                     res != ""
                                         ? "Successfully claimed " + res
                                         : "");
        }
        if (text.substring(0, 7) == "unclaim") {  // unclaim machine
            int unclaim_id = (text.substring(text.indexOf('-') + 1, text.length())).toInt();
            String res = rm.unclaim(unclaim_id, curr_user);
            bot->answerCallbackQuery(curr_msg.query_id,
                                     res != ""
                                         ? "Successfully unclaimed " + res
                                         : "You have not claimed this machine");
        }
    }

    /**
     * @brief Handle commands and messages.
     *
     * @param msg_number Message number as determined by UniversalTelegramBot::getUpdates().
     * @param rm The laundry room instance to reference.
     */
    void tg::handle_message(int msg_number, laundry::Room& rm) {
        auto& curr_msg = bot->messages[msg_number];
        String text = curr_msg.text;
        String chat_id = String(curr_msg.chat_id);
        if (is_command(text, config::COMMAND_START)) {
            String from_name = curr_msg.from_name;
            bot->sendMessage(chat_id, response_start(from_name), "");
        } else if (is_command(text, config::COMMAND_HELP)) {
            bot->sendMessage(chat_id, response_help(), config::MARKDOWN);
        } else if (is_command(text, config::COMMAND_CLAIM)) {
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               response_claim(),
                                               config::MARKDOWN,
                                               keyboard_claim(rm));
        } else if (is_command(text, config::COMMAND_STATUS)) {
            bot->sendMessage(chat_id, response_status(rm), config::MARKDOWN);
        } else if (is_command(text, config::COMMAND_UNCLAIM)) {
            auto claimed = rm.get_claimed_machines(curr_msg.from_id);
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               response_unclaim(claimed),
                                               config::MARKDOWN,
                                               keyboard_unclaim(claimed));
        }
    }

    /**
     * @brief Check if the given string is a valid command.
     *
     * @param msg String to be checked.
     * @param cmd Command to verify.
     * @return true Input matches command.
     * @return false Input does not match command, input is an invalid command
     */
    bool tg::is_command(const String& msg, const String& cmd) {
        return (cmd == msg || cmd == msg.substring(0, cmd.length()));
    }

    /**
     * @brief Generate an inline keyboard JSON string.
     *
     * @param buttons The inline keyboard buttons.
     * @return String InlineKeyboardMarkup JSON string.
     */
    String tg::generate_inline_keyboard(std::vector<inlineKeyboardButton>& buttons) {
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
    String tg::keyboard_claim(laundry::Room& rm) {
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
    String tg::keyboard_unclaim(std::vector<laundry::Machine>& claimed) {
        std::vector<inlineKeyboardButton> buttons;
        for (auto& m : claimed) {
            inlineKeyboardButton b = {
                .text = m.get_name(),
                .callback_data = "unclaim-" + String(m.id)};
            buttons.emplace_back(b);
        }
        return generate_inline_keyboard(buttons);
    }

    /**
     * @brief Response for /claim.
     *
     * @return String
     */
    String tg::response_claim() {
        return "Which machine would you like to claim?";
    }

    /**
     * @brief Response for /help.
     *
     * @return String
     */
    String tg::response_help() {
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
    String tg::response_start(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }

    /**
     * @brief Response for /status.
     *
     * @param rm The laundry room instance to reference.
     * @return String
     */
    String tg::response_status(laundry::Room& rm) {
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
    String tg::response_unclaim(std::vector<laundry::Machine>& claimed) {
        if (claimed.size() == 0) {
            return "You have not claimed any machines.";
        }

        return "Which machine would you like to unclaim?";
    }

    /**
     * @brief Send a message to the target ID.
     *
     * @param msg Message to be sent.
     * @param target Target ID (user, group, channel).
     * @param keyboard InlineKeyboardMarkup JSON string, optional.
     */
    void tg::send_message(String msg, String target, String keyboard) {
        bot->sendMessageWithInlineKeyboard(target, msg, config::MARKDOWN, keyboard);
    }
}
