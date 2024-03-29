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

                if (currMsg.type == CALLBACK_QUERY) {
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
        if (text == "restart-yes") {
            if (!isAuthorisedUser(currMsg.from_id)) {
                bot->answerCallbackQuery(currMsg.query_id, RESPONSE_UNAUTHORISED);
            } else {
                bot->answerCallbackQuery(currMsg.query_id, RESPONSE_RESTARTING);
                delay(config::MESSAGE_INTERVAL);
                ESP.restart();
            }
        }
        if (text == "restart-no") {
            bot->answerCallbackQuery(currMsg.query_id);
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
        if (isCommand(text, COMMAND_START)) {
            String from_name = currMsg.from_name;
            bot->sendMessage(chat_id, responseStart(from_name), "");
        } else if (isCommand(text, COMMAND_HELP)) {
            bot->sendMessage(chat_id, responseHelp(), MARKDOWN);
        } else if (isCommand(text, COMMAND_CLAIM)) {
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               responseClaim(),
                                               MARKDOWN,
                                               keyboardClaim(rm));
        } else if (isCommand(text, COMMAND_STATUS)) {
            bot->sendMessage(chat_id, responseStatus(rm), MARKDOWN);
        } else if (isCommand(text, COMMAND_UNCLAIM)) {
            auto claimed = rm.getClaimedMachines(currMsg.from_id);
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               responseUnclaim(claimed),
                                               MARKDOWN,
                                               keyboardUnclaim(claimed));
        } else if (isCommand(text, COMMAND_RESTART)) {
            if (!isAuthorisedUser(currMsg.from_id)) {
                bot->sendMessage(chat_id, RESPONSE_UNAUTHORISED);
                return;
            }

            // user is authorised, get restart confirmation
            bot->sendMessageWithInlineKeyboard(chat_id,
                                               RESPONSE_CONFIRM_RESTART,
                                               MARKDOWN,
                                               keyboardConfirm("restart"));
        } else if (isCommand(text, COMMAND_STATS)) {
            if (!isAuthorisedUser(currMsg.from_id)) {
                bot->sendMessage(chat_id, RESPONSE_UNAUTHORISED);
                return;
            }
            
            bot->sendMessage(chat_id, responseStats());
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
        bot->sendMessageWithInlineKeyboard(target, msg, MARKDOWN, keyboard);
    }

    /**
     * @brief Check if the user has elevated privileges
     *
     * @param userId Telegram User ID
     * @return true User is authorised
     * @return false User is unauthorised
     */
    bool tg::isAuthorisedUser(const String userId) {
        // Arduino string is not hashable, convert to std::string
        return (config::AUTHORISED_USERS.find(std::string(userId.c_str())) != config::AUTHORISED_USERS.end());
    }
}
