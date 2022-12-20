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
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }
}
