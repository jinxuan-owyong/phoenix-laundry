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
        if (text.substring(0, 5) == "claim") {  // claiming machine
            int claim_id = (text.substring(text.indexOf('-') + 1, text.length())).toInt();
            rm.claim(claim_id, curr_msg.from_name, curr_msg.from_id);
        }
    }
    void tg::handle_message(int msg_number, laundry::Room& rm) {
        auto& curr_msg = bot->messages[msg_number];
        String text = curr_msg.text;
        String chat_id = String(curr_msg.chat_id);
        if (text == constants.COMMAND_START) {
            String from_name = curr_msg.from_name;
            bot->sendMessage(chat_id, response_start(from_name), "");
        } else if (text == constants.COMMAND_HELP) {
            bot->sendMessage(chat_id, response_help(), constants.MARKDOWN);
        } else if (text == constants.COMMAND_CLAIM) {
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               response_claim(),
                                               constants.MARKDOWN,
                                               keyboard_claim(rm));
        } else if (text == constants.COMMAND_STATUS) {
            bot->sendMessage(chat_id, response_status(rm), constants.MARKDOWN);
        }
    }
    String tg::response_claim() {
        return "Which machine would you like to claim?";
    }

    String tg::keyboard_claim(laundry::Room& rm) {
        String keyboardJson = "[";
        for (int i = 0; i < rm.machines.size(); ++i) {
            keyboardJson += "[{\"text\" : ";
            keyboardJson += "\"" + rm.machines[i].get_name() + "\",";
            keyboardJson += "\"callback_data\" : ";
            keyboardJson += "\"claim-" + String(rm.machines[i].id) + "\"";
            keyboardJson += "}]";

            if (i != rm.machines.size() - 1) {
                keyboardJson += ",";
            }
        }
        keyboardJson += "]";
        return keyboardJson;
    }

    String tg::response_help() {
        String output = "*Laundry Bot Help Menu*\n";
        output += "Commands:\n";
        output += "/claim - Tag a dryer/washer to your telegram handle\n";
        output += "/help - Show this message\n";
        output += "/start - Initialise bot\n";
        output += "/status - Get the status of the laundry room";
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
}
