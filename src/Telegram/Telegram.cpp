#include "Telegram/Telegram.h"

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
    void tg::checkUpdates(laundry::Room& rm) {
        int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
        while (numNewMessages) {
            Serial.print("new message: ");
            Serial.println(String(numNewMessages));

            for (int i = 0; i < numNewMessages; i++) {
                auto& currMsg = bot->messages[i];
                // Chat id of the requester
                String chat_id = String(currMsg.chat_id);

                // Print the received message
                String text = currMsg.text;
                Serial.println(text);

                if (currMsg.type == config::CALLBACK_QUERY) {
                    handleCallback(i, rm);
                } else {
                    handleMessage(i, rm);
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
    void tg::handleCallback(int msg_number, laundry::Room& rm) {
        auto& currMsg = bot->messages[msg_number];
        String text = currMsg.text;
        laundry::User currUser(currMsg.from_name, currMsg.from_id, currMsg.username);
        if (text.substring(0, 5) == "claim") {  // claiming machine
            laundry::MachineID claimId = (laundry::MachineID)(text.substring(text.indexOf('-') + 1, text.length())).toInt();
            rm.claim(claimId, currUser);
            bot->answerCallbackQuery(currMsg.query_id, "Successfully claimed " + laundry::Machine::getNameById(claimId));
        }
        if (text.substring(0, 7) == "unclaim") {  // unclaim machine
            laundry::MachineID unclaimId = (laundry::MachineID)(text.substring(text.indexOf('-') + 1, text.length())).toInt();
            bool success = rm.unclaim(unclaimId, currUser);
            bot->answerCallbackQuery(currMsg.query_id,
                                     success
                                         ? "Successfully unclaimed " + laundry::Machine::getNameById(unclaimId)
                                         : "You have not claimed this machine");
        }
    }

    /**
     * @brief Handle commands and messages.
     *
     * @param msg_number Message number as determined by UniversalTelegramBot::getUpdates().
     * @param rm The laundry room instance to reference.
     */
    void tg::handleMessage(int msg_number, laundry::Room& rm) {
        auto& currMsg = bot->messages[msg_number];
        String text = currMsg.text;
        String chat_id = String(currMsg.chat_id);
        if (isCommand(text, config::COMMAND_START)) {
            String from_name = currMsg.from_name;
            bot->sendMessage(chat_id, responseStart(from_name), "");
        } else if (isCommand(text, config::COMMAND_HELP)) {
            bot->sendMessage(chat_id, responseHelp(), config::MARKDOWN);
        } else if (isCommand(text, config::COMMAND_CLAIM)) {
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               responseClaim(),
                                               config::MARKDOWN,
                                               keyboardClaim(rm));
        } else if (isCommand(text, config::COMMAND_STATUS)) {
            bot->sendMessage(chat_id, responseStatus(rm), config::MARKDOWN);
        } else if (isCommand(text, config::COMMAND_UNCLAIM)) {
            auto claimed = rm.getClaimedMachines(currMsg.from_id);
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               responseUnclaim(claimed),
                                               config::MARKDOWN,
                                               keyboardUnclaim(claimed));
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
    bool tg::isCommand(const String& msg, const String& cmd) {
        return (cmd == msg || cmd == msg.substring(0, cmd.length()));
    }

    /**
     * @brief Send a message to the target ID.
     *
     * @param msg Message to be sent.
     * @param target Target ID (user, group, channel).
     * @param keyboard InlineKeyboardMarkup JSON string, optional.
     */
    void tg::sendMessage(String msg, String target, String keyboard) {
        bot->sendMessageWithInlineKeyboard(target, msg, config::MARKDOWN, keyboard);
    }
}
