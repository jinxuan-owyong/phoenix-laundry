#include "Telegram.h"

namespace telegram {
    config::cfg constants;
    tg::tg(const char* token, WiFiClientSecure& client) {
        bot = new UniversalTelegramBot(token, client);
    }

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

                if (curr_msg.type == constants.CALLBACK_QUERY) {
                    handle_callback(i, rm);
                } else {
                    handle_message(i, rm);
                }
            }
            numNewMessages = bot->getUpdates(bot->last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }

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

    void tg::handle_message(int msg_number, laundry::Room& rm) {
        auto& curr_msg = bot->messages[msg_number];
        String text = curr_msg.text;
        String chat_id = String(curr_msg.chat_id);
        if (is_command(text, constants.COMMAND_START)) {
            String from_name = curr_msg.from_name;
            bot->sendMessage(chat_id, response_start(from_name), "");
        } else if (is_command(text, constants.COMMAND_HELP)) {
            bot->sendMessage(chat_id, response_help(), constants.MARKDOWN);
        } else if (is_command(text, constants.COMMAND_CLAIM)) {
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               response_claim(),
                                               constants.MARKDOWN,
                                               keyboard_claim(rm));
        } else if (is_command(text, constants.COMMAND_STATUS)) {
            bot->sendMessage(chat_id, response_status(rm), constants.MARKDOWN);
        } else if (is_command(text, constants.COMMAND_UNCLAIM)) {
            auto claimed = rm.get_claimed_machines(curr_msg.from_id);
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               response_unclaim(claimed),
                                               constants.MARKDOWN,
                                               keyboard_unclaim(claimed));
        }
    }

    bool tg::is_command(const String& msg, const String& cmd) {
        return (cmd == msg || cmd == msg.substring(0, cmd.length()));
    }

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

    String tg::response_claim() {
        return "Which machine would you like to claim?";
    }

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

    String tg::response_start(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }

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

    String tg::response_unclaim(std::vector<laundry::Machine>& claimed) {
        if (claimed.size() == 0) {
            return "You have not claimed any machines.";
        }

        return "Which machine would you like to unclaim?";
    }

    void tg::send_message(String msg, String target, String keyboard) {
        bot->sendMessageWithInlineKeyboard(target, msg, constants.MARKDOWN, keyboard);
    }
}
