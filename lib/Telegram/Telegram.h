#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>

namespace telegram {
    const unsigned long BOT_MTBS = 1000;  // mean time between scan messages

    const String COMMAND_HELP = "/help";
    const String COMMAND_START = "/start";
    const String MARKDOWN = "Markdown";

    String response_help();
    String response_start(String name);
}

#endif  // TELEGRAM_H
