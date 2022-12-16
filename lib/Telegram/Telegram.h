#ifndef TELEGRAM_H
#define TELEGRAM_H

#include "ArduinoJson.h"
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"

namespace telegram {
    const unsigned long BOT_MTBS = 1000;  // mean time between scan messages

    const String COMMAND_START = "/start";
    const String COMMAND_HELP = "/help";
    const String MARKDOWN = "Markdown";

    String response_help();
    String response_start(String name);

    void handleNewMessages(UniversalTelegramBot bot, int numNewMessages);
    void check_updates(UniversalTelegramBot bot);
}

#endif  // TELEGRAM_H
