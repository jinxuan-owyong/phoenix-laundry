#include "Telegram.h"

namespace telegram {
    String response_help() {
        return "\
*Laundry Bot Help Menu*\n\
Commands:\n\
/start - Initialise bot\n\
/help - Show this message\n";
    }
    String response_start(String name) {
        return "Hello " + name;
    }

    void handleNewMessages(UniversalTelegramBot bot, int numNewMessages) {
        Serial.println(String(numNewMessages));

        for (int i = 0; i < numNewMessages; i++) {
            // Chat id of the requester
            String chat_id = String(bot.messages[i].chat_id);

            // Print the received message
            String text = bot.messages[i].text;
            Serial.println(text);

            if (text == COMMAND_START) {
                String from_name = bot.messages[i].from_name;
                bot.sendMessage(chat_id, response_start(from_name), "");
            } else if (text == COMMAND_HELP) {
                bot.sendMessage(chat_id, response_help(), MARKDOWN);
            }
        }
    }
    void check_updates(UniversalTelegramBot bot) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
            Serial.print("new message: ");
            handleNewMessages(bot, numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
    }
}
